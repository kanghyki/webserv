#include "./Server.hpp"
#include "./http/HttpRequest.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Server::Server(Config& config) :
  requests(MANAGE_FD_MAX),
  recvs(MANAGE_FD_MAX),
  fdMax(FD_CLOSED),
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
  // FIXME:
  if (sock == SOCK_CLOSED)
    throw Server::InitException();

  return sock;
}

inline void Server::socketaddrInit(const std::string& host, int port, sock& in) {
  // FIXME:
  if (!memset(&in, 0, sizeof(in)))
    throw Server::InitException();
  in.sin_family = AF_INET;
  inet_pton(AF_INET, host.c_str(), &(in.sin_addr));
  in.sin_port = htons(port);

  log::info << "Preparing... Host=[" << inet_ntoa(in.sin_addr) << "] Port=[" << ntohs(in.sin_port) << "]" << log::endl;
}

inline void Server::socketOpen(int servFd, sock& in) {
  bool  bind_success = false;
  bool  listen_success = false;

  for (int i = 0; i < 10; ++i) {
    if (bind(servFd, (struct sockaddr*)&in, sizeof(in)) == -1)
      log::warning << "Bind failed... retry... " << i + 1 << log::endl;
    else {
      bind_success = true;
      break;
    }
    sleep(6);
  }

  if (bind_success == false)
    throw Server::BindException();


  for (int i = 0; i < 10; ++i) {
    if (listen(servFd, MANAGE_FD_MAX) == -1)
      log::warning << "Listen failed... retry... " << i + 1 << log::endl;
    else {
      listen_success = true;
      break;
    }
    sleep(6);
  }

  if (listen_success == false)
    throw Server::ListenException();

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
      log::error << "select -1" << log::endl;
      break;
    }

    cleanUpConnection();

    for (int i = 0; i < this->getFdMax() + 1; i++) {

      if (FD_ISSET(i, &this->writes)) {
        if (FD_ISSET(i, &writesCpy)) {
          if (this->requests[i].getHeader().getConnection() == HttpRequestHeader::CLOSE) {
            FD_CLR(i, &this->getWrites());
            FD_CLR(i, &this->getReads());
            this->connection.remove(i);
            this->connection.removeRequests(i);
            // FIXME:
            if (close(i) == -1)
              log::warning << "close -1" << log::endl;
            else
              log::info << "Closed client(" << i << ")" << log::endl;
          }
          else {
            FD_CLR(i, &this->writes);
            this->connection.update(i, this->requests[i].getServerConfig());
          }
          this->requests[i] = HttpRequest();
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
      log::warning << "close -1" << log::endl;
}


int Server::acceptConnect(int server_fd) {
  struct sockaddr_in  client_addr;
  socklen_t           size;

  size = sizeof(client_addr);
  int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &size);
  if (client_fd == -1 || fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
    log::warning << "connection failed" << log::endl;
    return FD_CLOSED;
  }

  FD_SET(client_fd, &this->getReads());
  if (this->getFdMax() < client_fd)
    this->setFdMax(client_fd);


  log::info << "Accept client(" << client_fd << ", " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << ") into (" << server_fd << ")" << log::endl;
  this->connection.update(client_fd, Connection::HEADER);

  return client_fd;
}

void Server::receiveData(int fd) {
  char buf[BUF_SIZE + 1];
  int recv_size;

  recv_size = recv(fd, buf, BUF_SIZE, 0);
  if (recv_size <= 0) {

    // for debug
    if (recv_size < 0)
      log::warning << "recv < 0" << log::endl;
    else
      log::warning << "recv == 0 client(" << fd  << ") was disconnetd" << log::endl;

    FD_CLR(fd, &this->reads);
    this->recvs[fd].clear();
    this->requests[fd] = HttpRequest();
    this->connection.remove(fd);
    this->connection.removeRequests(fd);
    if (close(fd) == -1)
      log::warning << "close -1" << log::endl;
    return ;
  }
  buf[recv_size] = 0;
  this->recvs[fd] += buf;
//  log::debug << "========================" << log::endl;
//  log::debug << this->recvs[fd] << log::endl;
//  log::debug << "recv:" << recv_size << log::endl;
//  log::debug << buf << log::endl;
//  log::debug << "========================" << log::endl;
  log::debug << "recvs[fd]size:" << this->recvs[fd].length() << log::endl;
  checkReceiveDone(fd);
}

void Server::checkReceiveDone(int fd) {
  HttpRequest& req = this->requests[fd];

  log::debug << "HttpRequest::recvStatus = ";
  if (req.getRecvStatus() == HttpRequest::HEADER_RECEIVE)
    log::debug << "HEADER" << log::endl;
  if (req.getRecvStatus() == HttpRequest::BODY_RECEIVE)
    log::debug << "BODY" << log::endl;
  if (req.getRecvStatus() == HttpRequest::RECEIVE_DONE)
    log::debug << "DONE" << log::endl;

  // @@@@ Header 받는 부분
  if (req.getRecvStatus() == HttpRequest::HEADER_RECEIVE)
    recvHeader(fd, req);


  // @@@@ Body 받는 부분
  if (req.getRecvStatus() == HttpRequest::BODY_RECEIVE) {
    log::debug << "body recieve!" << log::endl;
    log::debug << "Transfer_encoding = " << req.getHeader().getTransferEncoding() << log::endl;

    // chunked로 온 요청 처리
    if (req.getHeader().getTransferEncoding() == HttpRequestHeader::CHUNKED) {
      // FIXME: localhost:8000/r/n 을 찾음,, 헤더 잘라놔서 괜찮을 듯 이제
      // 종료 조건
      if (this->recvs[fd].find("0\r\n\r\n") != std::string::npos) {
        log::debug << "chunked find!" << log::endl;
        req.setRecvStatus(HttpRequest::RECEIVE_DONE);
      }
    }
    // 일반 요청 처리
    else if (req.getHeader().getTransferEncoding() == HttpRequestHeader::UNSET) {
        log::debug << "unchunked find!" << log::endl;
      // 종료 조건
      if (req.getContentLength() == (int)recvs[fd].length())
        req.setRecvStatus(HttpRequest::RECEIVE_DONE);
    }

  }

  // @@@@ 데이터 전부 전송됨
  if (req.getRecvStatus() == HttpRequest::RECEIVE_DONE) {
    log::debug << "recieve done!" << log::endl;

    // 지금까지 받은 데이터를 body 에 담기
    req.setBody(this->recvs[fd]);
    this->recvs[fd].clear();
    if (req.getHeader().getTransferEncoding() == HttpRequestHeader::CHUNKED) {
      try {
        req.unchunk();
      } catch (HttpStatus s) {
        log::warning << "Chunked message is wrong" << log::endl;
        req.setErrorStatus(BAD_REQUEST);
      }
    }

    // Response 만들러 ㄱ
    receiveDone(fd);
  }
}

void Server::recvHeader(int fd, HttpRequest& req) {
  log::debug << "recvHeader!" << log::endl;
  size_t pos = this->recvs[fd].find("\r\n\r\n");

  if (pos != std::string::npos) {
    log::debug << "header find!" << log::endl;

    std::string header = this->recvs[fd].substr(0, pos);
    std::string left = this->recvs[fd].substr(pos + 4);
    this->recvs[fd].clear();
    this->recvs[fd] = left;

    try {
      req.parse(header, this->config);
      this->connection.update(fd, Connection::BODY);
    } catch (HttpStatus s) {
      req.setRecvStatus(HttpRequest::RECEIVE_DONE);
      req.setErrorStatus(s);
      return;
    }

    // chunked 요청일 경우
    if (req.getHeader().getTransferEncoding() == HttpRequestHeader::CHUNKED) {
      log::debug << "It is chunked" << log::endl;
      req.setRecvStatus(HttpRequest::BODY_RECEIVE);
    }
    // 일반 요청일 경우
    else {
      std::string contentLength = req.getHeader().get(HttpRequestHeader::CONTENT_LENGTH);
      log::debug << "It is not chunked" << log::endl;
      // content 가 없을 경우
      if (contentLength.empty())
        req.setRecvStatus(HttpRequest::RECEIVE_DONE);
      // content 가 있을 경우
      else {
        req.setContentLength(std::atoi(contentLength.c_str()));
        req.setRecvStatus(HttpRequest::BODY_RECEIVE);
      }
    }
  }
  else {
    log::debug << "recvHeader \\r\\n\\r\\n not found" << log::endl;
  }
}

void Server::receiveDone(int fd) {
  HttpRequest&  req = this->requests[fd];
  HttpResponse  res;

  try {
    log::info << "=> Request from " << fd << " to " << req.getServerConfig().getServerName() << ", Method=\"" << req.getMethod() << "\" URI=\"" << req.getPath() << "\"" << log::endl;
    if (req.isError())
      throw req.getErrorStatus();
    else
      res = Http::processing(req, this->sessionManager);
  } catch (HttpStatus status) {
    res = Http::getErrorPage(status, req.getServerConfig());
  }

  int reqs = this->connection.updateRequests(fd, req.getServerConfig());
  res.addHeader("Keep-Alive", "timeout=" + util::itoa(req.getServerConfig().getKeepAliveTimeout()) + ", max=" + util::itoa(reqs));

  log::info << "<= Response to " << fd << " from " << req.getServerConfig().getServerName() << ", Status=" << res.getStatusCode() << log::endl;
  this->requests[fd] = HttpRequest();
  sendData(fd, res.toString());
}

void Server::sendData(int fd, const std::string& data) {
  this->connection.update(fd, Connection::SEND);
  FD_SET(fd, &this->writes);
  if (send(fd, data.c_str(), data.length(), 0) == SOCK_ERROR)
    log::warning << "send failed" << log::endl;
  this->requests[fd] = HttpRequest();
}

void Server::cleanUpConnection() {
  std::set<int> fd_list;

  fd_list = this->connection.getTimeoutList();
  for (std::set<int>::iterator it = fd_list.begin(); it != fd_list.end(); ++it) {
    log::info << "Timeout client(" << *it << "), closed" << log::endl;
    FD_CLR(*it, &this->getReads());
    FD_CLR(*it, &this->getWrites());
    close(*it);
    this->requests[*it] = HttpRequest();
    this->connection.remove(*it);
    this->connection.removeRequests(*it);
  }

  fd_list = this->connection.getMaxRequestList();
  for (std::set<int>::iterator it = fd_list.begin(); it != fd_list.end(); ++it) {
    log::info << "max request client(" << *it << "), closed" << log::endl;
    FD_CLR(*it, &this->getReads());
    FD_CLR(*it, &this->getWrites());
    close(*it);
    this->requests[*it] = HttpRequest();
    this->connection.remove(*it);
    this->connection.removeRequests(*it);
  }
}

const char* Server::InitException::what() const throw() {
  return "Server init failed";
}

const char* Server::BindException::what() const throw() {
  return "Server bind failed";
}

const char* Server::ListenException::what() const throw() {
  return "Server listen failed";
}
