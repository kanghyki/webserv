#include "Server.hpp"
#include "SessionManager.hpp"
#include "Util.hpp"
#include "http/HttpHeaderField.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Server::Server(Config& config) :
  requests(MANAGE_FD_MAX),
  fdMax(FD_CLOSED),
  config(config),
  connection(),
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

    if (select(this->getFdMax() + 1, &readsCpy, &writesCpy, 0, &t) == -1)
      break;

    cleanUpTimeout();

    for (int i = 0; i < this->getFdMax() + 1; i++) {
      if (FD_ISSET(i, &readsCpy)) {
        if (FD_ISSET(i, &this->listens))
          acceptConnect(i);
        else
          receiveData(i);
      }
      if (FD_ISSET(i, &writesCpy)) {
        if (this->requests[i].getReqType() == HttpRequest::CLOSE)
          closeSocket(i);
        if (this->requests[i].getReqType() == HttpRequest::CHUNKED) {

        }
        // 강현이 타임아웃 적용해서 keep alive 끊으면 되고, chunked 구현하면 될 듯?


      }

    }
  }

  for (size_t i = 0; i < this->listens_fd.size(); ++i)
    if (close(this->listens_fd[i]) == -1) throw (CloseException());
}

void Server::cleanUpTimeout() {
  std::vector<int> timeout_fd_list = this->connection.getTimeoutList();
  for (size_t i = 0; i < timeout_fd_list.size(); ++i) {
    log::warning << "Timeout client(" << timeout_fd_list[i] << "), closed" << log::endl;
    FD_CLR(timeout_fd_list[i], &this->getReads());
    FD_CLR(timeout_fd_list[i], &this->getWrites());
    close(timeout_fd_list[i]);
    clearReceived(timeout_fd_list[i]);
    this->connection.remove(timeout_fd_list[i]);
  }
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
  this->connection.add(client_fd);

  return client_fd;
}

void Server::receiveData(int fd) {
  char buf[BUF_SIZE + 1];
  int recv_size;

  recv_size = recv(fd, buf, BUF_SIZE, 0);
  // FIXME:
  if (recv_size < 0) {
    log::warning << "recv failed" << log::endl;
    throw (RecvException());
  }
  else if (recv_size == 0) {
    FD_CLR(fd, &this->reads);
    clearRequest(fd);
    // FIXME:
    if (close(fd) == -1)
      throw (CloseException());
    return ;
  }
  buf[recv_size] = 0;
  this->requests[fd].addRecvData(buf);
  checkReceiveDone(fd);
}

void Server::checkReceiveDone(int fd) {
  HttpRequest& req = this->requests[fd];

  if (req.getRecvStatus() == HttpRequest::HEADER_RECEIVE)
    recvHeader(req);

  if (req.getRecvStatus() == HttpRequest::BODY_RECEIVE) {
    if (req.getContentLength() == (int)req.getRecvData().length())
      req.setRecvStatus(HttpRequest::RECEIVE_DONE);
  }

  if (req.getRecvStatus() == HttpRequest::RECEIVE_DONE) 
    receiveDone(fd);
}

void Server::receiveDone(int fd) {
  HttpRequest&   req = this->requests[fd];
  HttpResponse  res;

//  FD_CLR(fd, &this->getReads());
  log::debug << "this->data[" << fd << "]\n" << this->requests[fd].getRecvData() << log::endl;
  this->connection.remove(fd);

  try {
    if (req.isError())
      throw req.getErrorStatus();
    req.setConfig(this->config);
    req.checkCGI(req.getPath(), req.getServerConfig());
    log::debug << "request good!" << log::endl;
    log::info << "=> Request from " << fd << " to " << req.getServerConfig().getServerName() << ", Method=\"" << req.getMethod() << "\" URI=\"" << req.getPath() << "\"" << log::endl;
    if (req.isCGI())
      res = Http::executeCGI(req, this->sessionManager);
    else
      res = Http::processing(req);
  } catch (HttpStatus status) {
    log::debug << "oh error!" << log::endl;
    res = Http::getErrorPage(status, req.getServerConfig());
  }

  log::info << "<= Response to " << fd << " from " << req.getServerConfig().getServerName() << ", Status=" << res.getStatusCode() << log::endl;
  sendData(fd, res.toString());
}

void Server::sendData(int fd, const std::string& data) {
  clearRequest(fd);
  FD_SET(fd, &this->writes);
  if (send(fd, data.c_str(), data.length(), 0) == SOCK_ERROR)
    log::warning << "send failed" << log::endl;
}

void Server::closeSocket(int fd) {
  FD_CLR(fd, &this->getWrites());
  // FIXME:
  if (close(fd) == -1)
    throw (CloseException());
  else
    log::info << "Closed client(" << fd << ")" << log::endl;
  this->requests[fd] = HttpRequest();
}

void Server::clearReceived(int fd) {
  HttpRequest& req = this->requests[fd];
  
  req.clearRecvData();
  req.setContentLength(0);
}

void Server::recvHeader(HttpRequest& req) {
  std::string recvData = req.getRecvData();
  size_t pos = recvData.find("\r\n\r\n");

  if (pos != std::string::npos) {
    std::string header = recvData.substr(0, pos);
    try {
      req.parseHeader(header);
      req.setReqType(req.getField(header_field::CONNECTION));
    } catch (HttpStatus s) {
      req.setRecvStatus(HttpRequest::RECEIVE_DONE);
      req.setErrorStatus(s);
      return;
    }
    std::string contentLength = req.getField(header_field::CONTENT_LENGTH);
    if (contentLength.empty()) {
      req.setRecvStatus(HttpRequest::RECEIVE_DONE);
      return;
    }
    else {
      req.setContentLength(std::atoi(contentLength.c_str()));
      req.setRecvData(recvData.substr(pos + 4));
      req.setRecvStatus(HttpRequest::BODY_RECEIVE);
    }
  }
}

void Server::clearRequest(int fd) {
  int tmp = this->requests[fd].getReqType();
  this->requests[fd] = HttpRequest();
  this->requests[fd].setReqType(tmp);
}

//const std::string Server::getData(int fd) const {
//  return this->recvTable[fd].data;
//}
//
//size_t Server::getContentLength(size_t fd) const {
//  return this->recvTable[fd].contentLength;
//}
//
//void Server::addData(int fd, const std::string& data) {
//  this->recvTable[fd].data += data;
//}
//
//void Server::setContentLength(int fd, int len) {
//  this->recvTable[fd].contentLength = len;
//}
//
//size_t Server::getHeaderPos(int fd) const {
//  return this->recvTable[fd].headerPos;
//}
//
//void Server::setHeaderPos(int fd, size_t pos) {
//  this->recvTable[fd].headerPos = pos;
//}
//
//
//int Server::getStatus(int fd) const {
//  return this->recvTable[fd].status;
//}
//
//void Server::setStatus(int fd, recvStatus status) {
//  this->recvTable[fd].status = status;
//}


const char* Server::InitException::what() const throw() {
  return "Server init failed";
}

const char* Server::BindException::what() const throw() {
  return "Server bind failed";
}

const char* Server::ListenException::what() const throw() {
  return "Server listen failed";
}

const char* Server::CloseException::what() const throw() {
  return "Server close failed";
}

const char* Server::RecvException::what() const throw() {
  return "Server recv failed";
}

/*
 * ---------------------- Non-Member Function ----------------------
 */
