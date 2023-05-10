#include "./Server.hpp"

const size_t        Server::BIND_MAX_TRIES = 10;
const size_t        Server::LISTEN_MAX_TRIES = 10;
const size_t        Server::TRY_SLEEP_TIME = 5;
const int           Server::BUF_SIZE = 1024 * 12;
const int           Server::MANAGE_FD_MAX = 1024;
const std::string   Server::HEADER_DELIMETER = "\r\n\r\n";
const std::string   Server::CHUNKED_DELIMETER = "0\r\n\r\n";

/*
 * -------------------------- Constructor --------------------------
 */

Server::Server(Config& config) :
  requests(MANAGE_FD_MAX),
  responses(MANAGE_FD_MAX),
  recvs(MANAGE_FD_MAX),
  fdMax(-1),
  config(config),
  connection(config),
  sessionManager() {
    FD_ZERO(&this->reads);
    FD_ZERO(&this->writes);
    FD_ZERO(&this->listens);

    std::vector<ServerConfig> servers = this->config.getHttpConfig().getServerConfig();
    for (std::vector<ServerConfig>::iterator sit = servers.begin(); sit != servers.end(); ++sit) {
      sockaddr_in sock;
      int fd = socketInit();

      socketaddrInit(sit->getHost(), sit->getPort(), sock);
      socketOpen(fd, sock);

      FD_SET(fd, &this->reads);
      FD_SET(fd, &this->listens);
      if (this->fdMax < fd)
        this->fdMax = fd;
      this->listens_fd.push_back(fd);
    }
}

/*
 * -------------------------- Destructor ---------------------------
 */

Server::~Server(void) {}

/*
 * -------------------------- Operator -----------------------------
 */

/*
 * ----------------------- Member Function -------------------------
 */

inline int Server::socketInit(void) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1)
    throw std::runtime_error("Server initialization failed");

  // for develop
  int option = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
  // -----------

  return fd;
}

inline void Server::socketaddrInit(const std::string& host, int port, sockaddr_in& in) {
  if (!memset(&in, 0, sizeof(in)))
    throw std::runtime_error("Server initialization failed");

  in.sin_family = AF_INET;
  inet_pton(AF_INET, host.c_str(), &(in.sin_addr));
  in.sin_port = htons(port);

  logger::info << "Preparing... Host=[" << inet_ntoa(in.sin_addr) << "] Port=[" << ntohs(in.sin_port) << "]" << logger::endl;
}

inline void Server::socketOpen(int servFd, sockaddr_in& in) {
  bool  bind_success = false;
  bool  listen_success = false;

  for (size_t i = 0; i < BIND_MAX_TRIES; ++i) {
    if (bind(servFd, (struct sockaddr*)&in, sizeof(in)) == -1)
      logger::warning << "Bind failed... retry... " << i + 1 << logger::endl;
    else {
      bind_success = true;
      break;
    }
    sleep(TRY_SLEEP_TIME);
  }

  if (bind_success == false)
    throw std::runtime_error("Server bind failed");

  for (size_t i = 0; i < LISTEN_MAX_TRIES; ++i) {
    if (listen(servFd, MANAGE_FD_MAX) == -1)
      logger::warning << "Listen failed... retry... " << i + 1 << logger::endl;
    else {
      listen_success = true;
      break;
    }
    sleep(TRY_SLEEP_TIME);
  }

  if (listen_success == false)
    throw std::runtime_error("Server listen failed");

  logger::info << "Listening... (" << servFd << ") \n\n\"http://" << inet_ntoa(in.sin_addr) << ":" << ntohs(in.sin_port) << "\"\n" << logger::endl;
}

inline void Server::fdSetInit(fd_set& fs, int fd) {
  FD_ZERO(&fs);
  FD_SET(fd, &fs);
}

void Server::run(void) {
  struct timeval t;

  t.tv_sec = 1;
  t.tv_usec = 0;
  logger::info << "Server is running..." << logger::endl;
  while (1) {

    fd_set readsCpy = this->reads;
    fd_set writesCpy = this->writes;

    if (select(this->fdMax + 1, &readsCpy, &writesCpy, 0, &t) == -1) {
      logger::error << "Select returns -1, break" << logger::endl;
      break;
    }

    cleanUpConnection();

    for (int i = 0; i < this->fdMax + 1; i++) {

      if (FD_ISSET(i, &this->writes)) {
        if (FD_ISSET(i, &writesCpy)) {
          HttpResponse::SendStatus send_status = this->responses[i].getSendStatus();
          if (send_status == HttpResponse::SENDING)
            sendData(i);
          else if (send_status == HttpResponse::DONE) {
            if (this->requests[i].getHeader().getConnection() == HttpRequestHeader::CLOSE)
              closeConnection(i);
            else
              keepAliveConnection(i);
          }
        }
      }
      else if (FD_ISSET(i, &readsCpy)) {
        if (FD_ISSET(i, &this->listens))
          acceptConnect(i);
        else
          receiveData(i);
      }
      else if (this->recvs[i].empty() == false)
        checkReceiveDone(i);
    }
  }

  for (size_t i = 0; i < this->listens_fd.size(); ++i)
    if (close(this->listens_fd[i]) == -1)
      logger::warning << "Closed, listen fd(" << i << ") with -1" << logger::endl;
}


void Server::acceptConnect(int server_fd) {
  struct sockaddr_in  client_addr;
  socklen_t           size;

  size = sizeof(client_addr);
  int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &size);
  if (client_fd == -1 || fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
    logger::warning << "Someone failed to accept request" << logger::endl;
    return ;
  }

  FD_SET(client_fd, &this->reads);
  if (this->fdMax < client_fd)
    this->fdMax = client_fd;


  logger::info << "Accept, client(" << client_fd << ", " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << ") into (" << server_fd << ")" << logger::endl;
  this->connection.update(client_fd, Connection::HEADER);

  return ;
}

void Server::receiveData(int fd) {
  char buf[BUF_SIZE + 1];
  int recv_size;

  recv_size = recv(fd, buf, BUF_SIZE, 0);
  if (recv_size <= 0) {
    if (recv_size < 0)
      logger::warning << "recv_size < 0 with client(" << fd  << ")" << logger::endl;
    closeConnection(fd);
    return ;
  }
  buf[recv_size] = 0;
//  logger::debug << "recv_size: " << recv_size << logger::endl;
//  logger::debug << "total: " << this->recvs[fd].length() << logger::endl;
  this->recvs[fd] += std::string(buf, recv_size);
  checkReceiveDone(fd);
}

void Server::checkReceiveDone(int fd) {
  HttpRequest& req = this->requests[fd];

  if (req.getRecvStatus() == HttpRequest::HEADER_RECEIVE)
    receiveHeader(fd, req);

  if (req.getRecvStatus() == HttpRequest::BODY_RECEIVE) {
    if (req.getHeader().getTransferEncoding() == HttpRequestHeader::CHUNKED) {

      size_t pos = this->recvs[fd].find(CHUNKED_DELIMETER);
      if (pos != std::string::npos) {
        req.setBody(this->recvs[fd].substr(0, pos));
        this->recvs[fd] = this->recvs[fd].substr(pos + CHUNKED_DELIMETER.length());
        req.setRecvStatus(HttpRequest::RECEIVE_DONE);
        try {
          req.unchunkBody();
        } catch (HttpStatus s) {
          logger::warning << "Chunked message is wrong" << logger::endl;
          req.setError(s);
        }
      }

    }
    else if (req.getHeader().getTransferEncoding() == HttpRequestHeader::UNSET) {

      if (req.getContentLength() <= static_cast<int>(recvs[fd].length())) {
        req.setBody(this->recvs[fd].substr(0, req.getContentLength()));
        this->recvs[fd] = this->recvs[fd].substr(req.getContentLength());
        req.setRecvStatus(HttpRequest::RECEIVE_DONE);
      }

    }
  }

  if (req.getRecvStatus() == HttpRequest::RECEIVE_DONE ||
      req.getRecvStatus() == HttpRequest::ERROR)
    receiveDone(fd);
}

void Server::receiveHeader(int fd, HttpRequest& req) {
  size_t pos = this->recvs[fd].find(HEADER_DELIMETER);

  if (pos != std::string::npos) {
    std::string header = this->recvs[fd].substr(0, pos);
    std::string left = this->recvs[fd].substr(pos + HEADER_DELIMETER.length());
    this->recvs[fd] = left;

    try {
      req.parse(header, this->config);
      logger::info << "Request from " << fd << " to " << req.getServerConfig().getServerName() << ", Method=\"" << req.getMethod() << "\" URI=\"" << req.getPath() << "\"" << logger::endl;
      this->connection.update(fd, Connection::BODY);
    } catch (HttpStatus s) {
      logger::debug << "Request header message is wrong" << logger::endl;
      req.setError(s);
      return;
    }

    if (req.getHeader().getTransferEncoding() == HttpRequestHeader::CHUNKED)
      req.setRecvStatus(HttpRequest::BODY_RECEIVE);
    else {
      std::string contentLength = req.getHeader().get(HttpRequestHeader::CONTENT_LENGTH);
      if (contentLength.empty())
        req.setRecvStatus(HttpRequest::RECEIVE_DONE);
      else {
        req.setContentLength(util::atoi(contentLength));
        req.setRecvStatus(HttpRequest::BODY_RECEIVE);
      }
    }
  }
}

void Server::receiveDone(int fd) {
  HttpRequest&  req = this->requests[fd];
  HttpResponse& res = this->responses[fd];

  try {
    res = Http::processing(req, this->sessionManager);
  } catch (HttpStatus s) {
    res = Http::getErrorPage(s, req);
  }

  if (req.getMethod() == request_method::HEAD) {
    res.getHeader().remove(HttpRequestHeader::CONTENT_TYPE);
    res.removeBody();
  }
  addExtraHeader(fd, req, res);
  this->connection.update(fd, Connection::SEND);
  FD_SET(fd, &this->writes);
  logger::info << "Response to " << fd << " from " << req.getServerConfig().getServerName() << ", Status=" << res.getStatusCode() << logger::endl;
  sendData(fd);
}

void Server::sendData(int fd) {
  HttpResponse& res = this->responses[fd];
  std::string   data = res.toString();
  int           send_size;

  if ((send_size = send(fd, data.c_str(), data.length(), 0)) <= 0) {
    if (send_size == -1)
      logger::warning << "send_size < 0 with client(" << fd  << ")" << logger::endl;
    closeConnection(fd);
  }
  else
    res.addSendLength(send_size);
}

void Server::addExtraHeader(int fd, HttpRequest& req, HttpResponse& res) {
  // connection
  HttpRequestHeader::connection connection = req.getHeader().getConnection();
  if (connection == HttpRequestHeader::KEEP_ALIVE) {
    int timeout = req.getServerConfig().getKeepAliveTimeout();
    int req_max = this->connection.updateRequests(fd, req.getServerConfig());
    if (req_max > 0) {
      res.getHeader().set(HttpResponseHeader::CONNECTION, "keep-alive");
      res.getHeader().set(HttpResponseHeader::KEEP_ALIVE, "timeout=" + util::itoa(timeout) + ", max=" + util::itoa(req_max));
    }
    else {
      req.setConnection(HttpRequestHeader::CLOSE); // for closeConnection
      res.getHeader().set(HttpResponseHeader::CONNECTION, "close");
    }
  }
  else if (connection == HttpRequestHeader::CLOSE)
    res.getHeader().set(HttpResponseHeader::CONNECTION, "close");

  // allow
  if (res.getStatusCode() == METHOD_NOT_ALLOWED)
    res.getHeader().set(HttpResponseHeader::ALLOW, req.getLocationConfig().toStringLimitExcept());

  if (res.getStatusCode() == UPGRADE_REQUIRED)
    res.getHeader().set(HttpResponseHeader::UPGRADE, "HTTP/1.1");
}

void Server::closeConnection(int fd) {
  if (FD_ISSET(fd, &this->writes)) FD_CLR(fd, &this->writes);
  if (FD_ISSET(fd, &this->reads)) FD_CLR(fd, &this->reads);
  if (fd == this->fdMax)
    this->fdMax -= 1;
  if (close(fd) == -1)
    logger::warning << "Closed, client(" << fd << ") with -1" << logger::endl;
  else
    logger::info << "Closed, client(" << fd << ")" << logger::endl;
  this->connection.remove(fd);
  this->connection.removeRequests(fd);
  this->requests[fd] = HttpRequest();
  this->responses[fd] = HttpResponse();
  this->recvs[fd] = "";
}

void Server::keepAliveConnection(int fd) {
  FD_CLR(fd, &this->writes);
  this->connection.update(fd, this->requests[fd].getServerConfig());
  this->requests[fd] = HttpRequest();
  this->responses[fd] = HttpResponse();
}

void Server::cleanUpConnection() {
  std::set<int> fd_list;

  fd_list = this->connection.getTimeoutList();
  for (std::set<int>::iterator it = fd_list.begin(); it != fd_list.end(); ++it) {
    logger::info << "Timeout, client(" << *it << ")" << logger::endl;
    closeConnection(*it);
  }
}
