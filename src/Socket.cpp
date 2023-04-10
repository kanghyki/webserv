#include "Socket.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Socket::Socket(const std::string& host, const int port) : host(host), port(port), \
                                                            servFd(SOCK_CLOSED), fdMax(FD_CLOSED) {
  this->servFd = socketInit();
  socketaddrInit(host, port, this->in);
  socketOpen(this->servFd, this->in);
  fdSetInit(this->reads, this->servFd);
  this->fdMax = this->servFd;
}


/*
 * -------------------------- Destructor ---------------------------
 */

Socket::~Socket(void) {}

/*
 * -------------------------- Operator -----------------------------
 */

/*
 * -------------------------- Getter -------------------------------
 */

const int Socket::getServFd(void) const {
  return this->servFd;
}

const int Socket::getFdMax(void) const {
  return this->fdMax;
}

fd_set& Socket::getReads(void) {
  return this->reads;
}

fd_set& Socket::getWrites(void) {
  return this->writes;
}

/*
 * -------------------------- Setter -------------------------------
 */

void Socket::setFdMax(int fdMax) {
  this->fdMax = fdMax;
}

/*
 * ----------------------- Member Function -------------------------
 */

int Socket::socketInit(void) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == SOCK_CLOSED)
    throw std::runtime_error("Socket init failed");

  return sock;
}

void Socket::socketaddrInit(const std::string& host, int port, sock& in) {
  (void)host;

  if (!memset(&in, 0, sizeof(in)))
    throw std::runtime_error("Socket init failed");
  in.sin_family = AF_INET;
  in.sin_addr.s_addr = htonl(INADDR_ANY);
  in.sin_port = htons(port);
}

void Socket::socketOpen(int servFd, sock& in) {
  if (bind(servFd, (struct sockaddr*)&in, sizeof(in)) == -1)
    throw std::runtime_error("Socket bind failed");
  if (listen(servFd, 128) == -1)
    throw std::runtime_error("Socket listen failed");
}

void Socket::fdSetInit(fd_set& fs, int fd) {
  FD_ZERO(&fs);
  FD_SET(fd, &fs);
}

void Socket::socketRun(void) {
  int clntFd = FD_CLOSED;

  while (1) {
    fd_set readsCpy = this->getReads();
    fd_set writesCpy = this->getWrites();

    if (select(this->getFdMax() + 1, &readsCpy, &writesCpy, 0, 0) == -1)
      break;

    for (int i = 0; i < this->getFdMax() + 1; i++) {
      if (FD_ISSET(i, &readsCpy))
        clntFd = handShake(i, clntFd);
      if (FD_ISSET(i, &writesCpy))
        closeSocket(i, clntFd);
    }
  }
  close(this->getServFd());
}

int Socket::acceptConnect() {
  int clntFd = accept(this->getServFd(), 0, 0);
  if (clntFd == -1 || fcntl(clntFd, F_SETFL, O_NONBLOCK) == -1) {
    std::cout << "[Log] connection failed: " << "\n";
    return FD_CLOSED;
  }

  std::cout << "[Log] connected client: " << clntFd << "\n";
  FD_SET(clntFd, &this->getReads());
  std::cout << "[Log] recevied data: " << recvData(clntFd) << "\n";
  if (this->getFdMax() < clntFd)
    this->setFdMax(clntFd);

  return clntFd;
}

std::string Socket::recvData(int fd) {
  char buf[BUF_SIZE + 1];
  size_t recv_size;
  std::string ret;

  while (1) {
    recv_size = recv(fd, buf, BUF_SIZE, 0);
    buf[BUF_SIZE] = 0;
    ret += buf;
    if (recv_size < BUF_SIZE)
      break;
  }
  shutdown(fd, SHUT_RD);

  return ret;
}

void Socket::sendData(int fd, int clntFd) {
  FD_CLR(fd, &this->getReads());
  FD_SET(clntFd, &this->getWrites());
  std::string data("HTTP/1.1 200 OK\r\n\
      Server: Hyeongki&Kanghyki server\r\n\
      Content-Length: 21\r\n\
      Content-Type: text/html\r\n\r\n\
      Hello, Webserv!");
  if (send(clntFd, data.c_str(), strlen(data.c_str()), 0) == SOCK_ERROR)
    std::cout << "[ERROR] send failed\n";
  else
    std::cout << "[Log] send data\n";
}

void Socket::closeSocket(int fd, int clntFd) {
  std::cout << "[Log] close\n";
  FD_CLR(fd, &this->getWrites());
  close(clntFd);
}

int Socket::handShake(int fd, int clntFd) {
  if (fd == this->getServFd())
    clntFd = acceptConnect();
  else
    sendData(fd, clntFd);

  return clntFd;
}

/*
 * ---------------------- Non-Member Function ----------------------
 */
