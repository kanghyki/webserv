#include "Socket.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Socket::Socket(ServerConfig config) : data(10000), host(config.getHost()), port(config.getPort()), \
                                                            servFd(SOCK_CLOSED), fdMax(FD_CLOSED) {
  this->servFd = socketInit();
  socketaddrInit(this->host, this->port, this->in);
  socketOpen(this->servFd, this->in);
  fdSetInit(this->reads, this->servFd);
  FD_ZERO(&this->writes);
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

inline int Socket::socketInit(void) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == SOCK_CLOSED)
    throw except::SocketInitException();

  return sock;
}

inline void Socket::socketaddrInit(const std::string& host, int port, sock& in) {
  (void)host;

  if (!memset(&in, 0, sizeof(in)))
    throw except::SocketInitException();
  in.sin_family = AF_INET;
  in.sin_addr.s_addr = htonl(INADDR_ANY);
  in.sin_port = htons(port);
}

inline void Socket::socketOpen(int servFd, sock& in) {
  if (bind(servFd, (struct sockaddr*)&in, sizeof(in)) == -1)
    throw except::SocketBindException();
  if (listen(servFd, 128) == -1)
    throw except::SocketListenException();
}

inline void Socket::fdSetInit(fd_set& fs, int fd) {
  FD_ZERO(&fs);
  FD_SET(fd, &fs);
}

void Socket::socketRun(void) {
  struct timeval t;
  t.tv_sec = 1;

  while (1) {
    fd_set readsCpy = this->getReads();
    fd_set writesCpy = this->getWrites();

    if (select(this->getFdMax() + 1, &readsCpy, &writesCpy, 0, &t) == -1)
      break;

    for (int i = 0; i < this->getFdMax() + 1; i++) {
      if (FD_ISSET(i, &readsCpy))
        handShake(i);
      if (FD_ISSET(i, &writesCpy))
        closeSocket(i);
    }
  }
  close(this->getServFd());
}

int Socket::acceptConnect() {
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


void Socket::receiveData(int fd) {
  char buf[BUF_SIZE + 1];
  int recv_size;
  int DONE = 0;

  recv_size = recv(fd, buf, BUF_SIZE, 0);
  if (recv_size == -1)
    return ;
  buf[recv_size] = 0;
  this->data[fd] += buf;
  // FIXME: 임시 조건
  if (recv_size < BUF_SIZE) {
    shutdown(fd, SHUT_RD);
    FD_CLR(fd, &this->getReads());

    std::cout << this->data[fd] << std::endl;
    this->data[fd] = "";
    // HttpRequest request(this->data[fd]);
    //
    // HttpDataFetcher fetcher(request);
    //
    // HttpResponseBuilder rb;
    // HttpResponse response = rb.setRequest(request).setFetcher(fecther).build();
    //
    // std::string ret = response.toString();
    // sendData(fd);

    sendData(fd);
    // 파싱하고 지지고 볶고, 데이터 전송
  }
}

void Socket::sendData(int fd) {
  FD_SET(fd, &this->getWrites());
  std::string data("HTTP/1.1 200 OK\r\n\
      Content-Length: 2048\r\n\
      Content-Type: text/html\r\n\r\n");
  data += util::readFile("./html/index.html");
  if (send(fd, data.c_str(), strlen(data.c_str()), 0) == SOCK_ERROR)
    std::cout << "[ERROR] send failed\n";
  else
    std::cout << "[Log] send data\n";
}

void Socket::closeSocket(int fd) {
  std::cout << "[Log] close\n";
  FD_CLR(fd, &this->getWrites());
  close(fd);
}

void Socket::handShake(int fd) {
  if (fd == this->getServFd())
    acceptConnect();
  else
    receiveData(fd);
}

/*
 * ---------------------- Non-Member Function ----------------------
 */
