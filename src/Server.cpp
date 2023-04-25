#include "Server.hpp"
#include "Util.hpp"
/* * -------------------------- Constructor --------------------------
 */

Server::Server(ServerConfig config) : data(MANAGE_FD_MAX), host(config.getHost()), port(config.getPort()), \
                                     servFd(SOCK_CLOSED), fdMax(FD_CLOSED), contentLengths(MANAGE_FD_MAX, -1), \
                                     headerPos(MANAGE_FD_MAX){
  this->servFd = socketInit();
  socketaddrInit(this->host, this->port, this->in);
  socketOpen(this->servFd, this->in);
  fdSetInit(this->reads, this->servFd);
  FD_ZERO(&this->writes);
  this->fdMax = this->servFd;
  this->config = config;
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

const int Server::getServFd(void) const {
  return this->servFd;
}

const int Server::getFdMax(void) const {
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
  if (sock == SOCK_CLOSED)
    throw Server::InitException();

  return sock;
}

inline void Server::socketaddrInit(const std::string& host, int port, sock& in) {
  if (!memset(&in, 0, sizeof(in)))
    throw Server::InitException();
  in.sin_family = AF_INET;
  inet_pton(AF_INET, host.c_str(), &(in.sin_addr));
  in.sin_port = htons(port);
}

inline void Server::socketOpen(int servFd, sock& in) {
  if (bind(servFd, (struct sockaddr*)&in, sizeof(in)) == -1)
    throw Server::BindException();
  if (listen(servFd, MANAGE_FD_MAX) == -1)
    throw Server::ListenException();
}

inline void Server::fdSetInit(fd_set& fs, int fd) {
  FD_ZERO(&fs);
  FD_SET(fd, &fs);
}

void Server::run(void) {
  struct timeval t;
  t.tv_sec = 1;
  t.tv_usec = 0;

  while (1) {
    fd_set readsCpy = this->getReads();
    fd_set writesCpy = this->getWrites();

    if (select(this->getFdMax() + 1, &readsCpy, &writesCpy, 0, &t) == -1)
      break;

    for (int i = 0; i < this->getFdMax() + 1; i++) {
      if (FD_ISSET(i, &readsCpy)) {
        if (i == this->getServFd())
          acceptConnect();
        else
          receiveData(i);
      }
      if (FD_ISSET(i, &writesCpy))
        closeSocket(i);
    }
  }
  close(this->getServFd());
}

int Server::acceptConnect() {
  int fd = accept(this->getServFd(), 0, 0);
  if (fd == -1 || fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
    std::cout << "[Log] connection failed: " << "\n";
    return FD_CLOSED;
  }
  std::cout << "[Log] connected client: " << fd << "\n";
  FD_SET(fd, &this->getReads());
  if (this->getFdMax() < fd)
    this->setFdMax(fd);

  return fd;
}


void Server::receiveData(int fd) {
  char buf[BUF_SIZE + 1];
  int recv_size;
  int DONE = 0;

  recv_size = recv(fd, buf, BUF_SIZE, 0);
  std::cout << "recv_size" << recv_size << std::endl;
  if (recv_size <= 0) {
    throw "error";
  }
  buf[recv_size] = 0;
  addData(fd, buf);
  checkContentLength(fd);
}

void Server::checkContentLength(int fd) {
  if (getContentLength(fd) == -1) {
    size_t pos = getData(fd).find("\r\n\r\n");
    if (pos != std::string::npos) {
      setContentLength(fd, 0);
      setHeaderPos(fd, pos);
    }
  }
  if (getContentLength(fd) == 0) {
    size_t start = util::toLowerStr(getData(fd)).find("content-length: "); 
    if (start != std::string::npos) {
      size_t end = getData(fd).find("\r\n", start);
      setContentLength(fd, std::atoi(this->data[fd].substr(start + 16, end - start + 1).c_str()));
    }
    else
      receiveDone(fd);
  }
  if (getContentLength(fd) > 0) {
    if (getContentLength(fd) == getData(fd).substr(getHeaderPos(fd) + 4).length())
      receiveDone(fd);
  }
}

#include "./http/Http.hpp"
#include "./http/HttpStatus.hpp"

void Server::sendData(int fd) {
  Http http(this->config);
  std::string response;
  FD_SET(fd, &this->getWrites());

  try {
    HttpRequest req(this->data[fd]);
    clearReceived(fd);
    response = http.processing(req);
  } catch (std::exception &e) {
    // TODO:: Error page
    response = "HTTP/1.1 500 " + getStatusText(INTERNAL_SERVER_ERROR) + "\r\n\r\n";
  }

  if (send(fd, response.c_str(), response.length(), 0) == SOCK_ERROR)
    std::cout << "[ERROR] send failed\n";
  else
    std::cout << "[Log] send data\n";
}

void Server::closeSocket(int fd) {
  std::cout << "[Log] close\n";
  FD_CLR(fd, &this->getWrites());
  close(fd);
}

void Server::receiveDone(int fd) {
  shutdown(fd, SHUT_RD);;
  FD_CLR(fd, &this->getReads());
  std::cout << "@---this->data[" << fd << "]" << std::endl;
  std::cout << this->data[fd];
  std::cout << "@---" << std::endl;
  sendData(fd);
  this->data[fd] = "";
}

const std::string Server::getData(int fd) const {
  return this->data[fd];
}

const int         Server::getContentLength(int fd) const {
  return this->contentLengths[fd];
}

void              Server::addData(int fd, const std::string& data) {
  this->data[fd] += data;
}

void              Server::setContentLength(int fd, int len) {
  this->contentLengths[fd] = len;
}

void              Server::clearData(int fd) {
  this->data[fd].clear();
}

void              Server::clearContentLength(int fd) {
  this->contentLengths[fd] = -1;
}

const size_t      Server::getHeaderPos(int fd) const {
  return this->headerPos[fd];
}

void              Server::setHeaderPos(int fd, size_t pos) {
  this->headerPos[fd] = pos;
}

void              Server::clearHeaderPos(int fd) {
  this->headerPos[fd] = 0;
}

void              Server::clearReceived(int fd) {
  clearData(fd);
  clearContentLength(fd);
  clearHeaderPos(fd);
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
/*
 * ---------------------- Non-Member Function ----------------------
 */
