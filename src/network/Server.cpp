#include "./Server.hpp"

const size_t        Server::BIND_MAX_TRIES = 10;
const size_t        Server::LISTEN_MAX_TRIES = 10;
const size_t        Server::TRY_SLEEP_TIME = 5;
//const int           Server::BUF_SIZE = 1024 * 12;
const int           Server::BUF_SIZE = 1024;
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
 * -------------------------- Getter -------------------------------
 */

//int Server::getServFd(void) const {
//  return this->servFd;
//}

int Server::getFdMax(void) const {
  return this->fdMax;
}

fd_set& Server::getReads(void) {
  return this->reads;
}

fd_set& Server::getWrites(void) {
  return this->writes;
}

/*
 * -------------------------- Setter -------------------------------
 */

void Server::setFdMax(int fdMax) {
  this->fdMax = fdMax;
}

/*
 * ----------------------- Member Function -------------------------
 */

inline int Server::socketInit(void) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1)
    throw std::runtime_error("Server initialization failed");

  return sock;
}

inline void Server::socketaddrInit(const std::string& host, int port, sockaddr_in& in) {
  if (!memset(&in, 0, sizeof(in)))
    throw std::runtime_error("Server initialization failed");

  in.sin_family = AF_INET;
  inet_pton(AF_INET, host.c_str(), &(in.sin_addr));
  in.sin_port = htons(port);

  log::info << "Preparing... Host=[" << inet_ntoa(in.sin_addr) << "] Port=[" << ntohs(in.sin_port) << "]" << log::endl;
}

inline void Server::socketOpen(int servFd, sockaddr_in& in) {
  bool  bind_success = false;
  bool  listen_success = false;

  for (size_t i = 0; i < BIND_MAX_TRIES; ++i) {
    if (bind(servFd, (struct sockaddr*)&in, sizeof(in)) == -1)
      log::warning << "Bind failed... retry... " << i + 1 << log::endl;
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
      log::warning << "Listen failed... retry... " << i + 1 << log::endl;
    else {
      listen_success = true;
      break;
    }
    sleep(TRY_SLEEP_TIME);
  }

  if (listen_success == false)
    throw std::runtime_error("Server listen failed");

  log::info << "Listening... (" << servFd << ") \n\n\"http://" << inet_ntoa(in.sin_addr) << ":" << ntohs(in.sin_port) << "\"\n" << log::endl;
}

inline void Server::fdSetInit(fd_set& fs, int fd) {
  FD_ZERO(&fs);
  FD_SET(fd, &fs);
}

void Server::run(void) {
  struct timeval t;

  t.tv_sec = 1;
  t.tv_usec = 0;
  log::info << "Server is running..." << log::endl;
  while (1) {

    fd_set readsCpy = this->getReads();
    fd_set writesCpy = this->getWrites();

    if (select(this->getFdMax() + 1, &readsCpy, &writesCpy, 0, &t) == -1) {
      log::error << "Select returns -1, break" << log::endl;
      break;
    }

    cleanUpConnection();

    for (int i = 0; i < this->getFdMax() + 1; i++) {

      if (FD_ISSET(i, &this->writes)) {

        if (FD_ISSET(i, &writesCpy)) {
          HttpResponse::sendStatus send_status = this->responses[i].getSendStatus();
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
    }
  }

  for (size_t i = 0; i < this->listens_fd.size(); ++i)
    if (close(this->listens_fd[i]) == -1)
      log::warning << "Closed, listen fd(" << i << ") with -1" << log::endl;
}


void Server::acceptConnect(int server_fd) {
  struct sockaddr_in  client_addr;
  socklen_t           size;

  size = sizeof(client_addr);
  int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &size);
  if (client_fd == -1 || fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
    log::warning << "Someone failed to accept request" << log::endl;
    return ;
  }

  FD_SET(client_fd, &this->getReads());
  if (this->getFdMax() < client_fd)
    this->setFdMax(client_fd);


  log::info << "Accept, client(" << client_fd << ", " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << ") into (" << server_fd << ")" << log::endl;
  this->connection.update(client_fd, Connection::HEADER);

  return ;
}

void Server::receiveData(int fd) {
  char buf[BUF_SIZE + 1];
  int recv_size;

  recv_size = recv(fd, buf, BUF_SIZE, 0);
  if (recv_size <= 0) {
    if (recv_size < 0)
      log::warning << "recv_size < 0 with client(" << fd  << ")" << log::endl;
    closeConnection(fd);
    return ;
  }
  buf[recv_size] = 0;
  this->recvs[fd] += std::string(buf, recv_size);
  checkReceiveDone(fd);
}

void Server::checkReceiveDone(int fd) {
  HttpRequest& req = this->requests[fd];

  if (req.getRecvStatus() == HttpRequest::HEADER_RECEIVE)
    receiveHeader(fd, req);

  if (req.getRecvStatus() == HttpRequest::BODY_RECEIVE) {
    if (req.getHeader().getTransferEncoding() == HttpRequestHeader::CHUNKED) {
      if (this->recvs[fd].find(CHUNKED_DELIMETER) != std::string::npos)
        req.setRecvStatus(HttpRequest::RECEIVE_DONE);
    }
    else if (req.getHeader().getTransferEncoding() == HttpRequestHeader::UNSET) {
      if (req.getContentLength() == static_cast<int>(recvs[fd].length()))
        req.setRecvStatus(HttpRequest::RECEIVE_DONE);
    }
  }

  if (req.getRecvStatus() == HttpRequest::RECEIVE_DONE) {
    req.setBody(this->recvs[fd]);
    if (req.getHeader().getTransferEncoding() == HttpRequestHeader::CHUNKED) {
      try {
        req.unchunkBody();
      } catch (HttpStatus s) {
        log::warning << "Chunked message is wrong" << log::endl;
        req.setError(s);
      }
    }
    receiveDone(fd);
  }

  if (req.getRecvStatus() == HttpRequest::ERROR)
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
      log::info << "Request from " << fd << " to " << req.getServerConfig().getServerName() << ", Method=\"" << req.getMethod() << "\" URI=\"" << req.getPath() << "\"" << log::endl;
      this->connection.update(fd, Connection::BODY);
    } catch (HttpStatus s) {
      log::warning << "Request header message is wrong" << log::endl;
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
        req.setContentLength(std::atoi(contentLength.c_str()));
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
    res = Http::getErrorPage(s, req.getServerConfig());
  }

  int timeout = req.getServerConfig().getKeepAliveTimeout();
  int req_max = this->connection.updateRequests(fd, req.getServerConfig());

  // connection
  HttpRequestHeader::connection connection = req.getHeader().getConnection();
  if (connection == HttpRequestHeader::KEEP_ALIVE)
    res.getHeader().set(HttpResponseHeader::CONNECTION, "keep-alive");
  else if (connection == HttpRequestHeader::CLOSE)
    res.getHeader().set(HttpResponseHeader::CONNECTION, "close");
  // keep-alive
  res.getHeader().set(HttpResponseHeader::KEEP_ALIVE, "timeout=" + util::itoa(timeout) + ", max=" + util::itoa(req_max));

  // allow
  if (res.getStatusCode() == METHOD_NOT_ALLOWED) {
    std::vector<std::string> vs = req.getLocationConfig().getLimitExcept();
    std::string value;

    for (size_t i = 0; i < vs.size(); ++i) {
      value += vs[i];
      if (i + 1 < vs.size())
        value += ", ";
    }
    res.getHeader().set(HttpResponseHeader::ALLOW, value);
  }

  if (res.getStatusCode() == UPGRADE_REQUIRED)
    res.getHeader().set(HttpResponseHeader::UPGRADE, "HTTP/1.1");

  log::info << "Response to " << fd << " from " << req.getServerConfig().getServerName() << ", Status=" << res.getStatusCode() << log::endl;
  this->connection.update(fd, Connection::SEND);
  FD_SET(fd, &this->writes);
  sendData(fd);
}

void Server::sendData(int fd) {
  HttpResponse& res = this->responses[fd];
  std::string   data = res.toString();
  int           ret;

  if ((ret = send(fd, data.c_str(), data.length(), 0)) == -1) {
    closeConnection(fd);
    log::warning << "send failed" << log::endl;
  }
  else
    res.addSendLength(ret);
}

void Server::closeConnection(int fd) {
  if (FD_ISSET(fd, &this->getWrites())) FD_CLR(fd, &this->getWrites());
  if (FD_ISSET(fd, &this->getReads())) FD_CLR(fd, &this->getReads());
  if (close(fd) == -1)
    log::warning << "Closed, client(" << fd << ") with -1" << log::endl;
  else
    log::info << "Closed, client(" << fd << ")" << log::endl;
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
  this->recvs[fd] = "";
}

void Server::cleanUpConnection() {
  std::set<int> fd_list;

  fd_list = this->connection.getTimeoutList();
  for (std::set<int>::iterator it = fd_list.begin(); it != fd_list.end(); ++it) {
    closeConnection(*it);
    log::info << "Timeout, client(" << *it << ") closed" << log::endl;
  }

  fd_list = this->connection.getMaxRequestList();
  for (std::set<int>::iterator it = fd_list.begin(); it != fd_list.end(); ++it) {
    closeConnection(*it);
    log::info << "Exceeded max request, client(" << *it << ") closed" << log::endl;
  }
}
