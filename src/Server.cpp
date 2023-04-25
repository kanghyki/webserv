#include "Server.hpp"
/* * -------------------------- Constructor --------------------------
 */

Server::Server(ServerConfig config) : data(1024), host(config.getHost()), port(config.getPort()), \
                                                            servFd(SOCK_CLOSED), fdMax(FD_CLOSED) {
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

const std::map<int, time_t>& Server::getTimeRecord() const {
  return this->timeout;
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
  if (listen(servFd, 128) == -1)
    throw Server::ListenException();
}

inline void Server::fdSetInit(fd_set& fs, int fd) {
  FD_ZERO(&fs);
  FD_SET(fd, &fs);
}

void Server::removeData(int fd) {
  this->data[fd].clear();
}

void Server::removeTimeRecord(int fd) {
  this->timeout.erase(fd);
}

bool Server::existsTimeRecord(int fd) {
  if (this->timeout.find(fd) != this->timeout.end())
    return true;
  return false;
}

void Server::appendTimeRecord(int fd) {
  time_t initTime;

  time(&initTime);
  this->timeout.insert(std::make_pair(fd, initTime));
}

void Server::DisconnectTimeoutClient() {
  const std::map<int, time_t>&  record = getTimeRecord();
  std::vector<int>              removeList;

  for (std::map<int, time_t>::const_iterator it = record.begin(); it != record.end(); ++it) {
    time_t reqTime = it->second;
    time_t curTime;

    time(&curTime);
    if (curTime - reqTime > TIMEOUT_MAX) {
      int fd = it->first;

      // 408 error
      FD_CLR(fd, &this->getReads());
      close(fd);
      std::cout << "timeout client: " << fd << std::endl;
      removeList.push_back(fd);
    }
  }

  for (int i = 0; i < removeList.size(); ++i) {
    removeTimeRecord(removeList[i]);
    removeData(removeList[i]);
  }
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

    DisconnectTimeoutClient();

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

  if (existsTimeRecord(fd) == false)
    appendTimeRecord(fd);

  recv_size = recv(fd, buf, BUF_SIZE, 0);
  std::cout << "recv_size" << recv_size << std::endl;
  if (recv_size <= 0) {
    throw "error";
  }
  buf[recv_size] = 0;
  this->data[fd] += buf;

  size_t pos = this->data[fd].find("\r\n\r\n");
  if (pos != std::string::npos) {
    std::string header;
    std::string body;

    size_t start = this->data[fd].find("Content-Length: ");
    if (start != std::string::npos) {
      size_t end = this->data[fd].find("\r\n", start);
      int len = std::atoi(this->data[fd].substr(start, end - start + 1).c_str());
      if (len == this->data[fd].substr(pos + 4).length()) {
        // 헤더 파싱해야됨
        header = this->data[fd].substr(0, pos);
        body = this->data[fd].substr(pos + 4);
        std::cout << "header : " << header << std::endl;
        std::cout << "body : " << body << std::endl;
        receiveDone(fd, header, body);
      }
      else
        return;
    }
    // 헤더 파싱해여됨
    header = this->data[fd].substr(0, pos);
    std::cout << "header : " << header << std::endl;
    std::cout << "body : " << body << std::endl;
    receiveDone(fd, header, body);
  }

//  if (recv_size < BUF_SIZE) {
//    shutdown(fd, SHUT_RD);
//    FD_CLR(fd, &this->getReads());
//    std::cout << "@---this->data[" << fd << "]" << std::endl;
//    std::cout << this->data[fd];
//    std::cout << "@---" << std::endl;
//    sendData(fd);
//    this->data[fd] = "";
////    close(fd);
////    FD_CLR(fd, &this->getReads());
//    return ;
//  }
}

#include "./http/Http.hpp"
#include "./http/HttpStatus.hpp"

void Server::sendData(int fd, std::string header, std::string body) {
  Http http(this->config);
  std::string response;
  FD_SET(fd, &this->getWrites());

  try {
    response = http.processing(this->data[fd]);
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

void Server::receiveDone(int fd, std::string header, std::string body) {
    shutdown(fd, SHUT_RD);
    FD_CLR(fd, &this->getReads());
    std::cout << "@---this->data[" << fd << "]" << std::endl;
    std::cout << this->data[fd];
    std::cout << "@---" << std::endl;
    sendData(fd, header, body);
    this->data[fd] = "";
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
