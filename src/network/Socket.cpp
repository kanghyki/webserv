#include "Socket.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Socket::Socket(ServerConfig config) : receivedData(1000), host(config.getHost()), port(config.getPort()), \
                                                            servFd(SOCK_CLOSED), fdMax(FD_CLOSED) {
  this->servFd = socketInit();
  socketaddrInit(this->host, this->port, this->in);
  socketOpen(this->servFd, this->in);
  fdSetInit(this->reads, this->servFd);
  FD_ZERO(&this->writes);
  this->setFdMax(this->servFd);
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

fd_set Socket::getReads(void) {
  return this->reads;
}

fd_set Socket::getWrites(void) {
  return this->writes;
}

const int Socket::getReadFd(void) const {
  return this->readFd;
}

const int Socket::getWriteFd(void) const {
  return this->writeFd;
}

/*
 * -------------------------- Setter -------------------------------
 */

void Socket::setFdMax(int fdMax) {
  this->fdMax = fdMax;
}

void Socket::setReadFd(int fd) {
  this->readFd = fd;
}

void Socket::setWriteFd(int fd) {
  this->writeFd = fd;
}

/*
 * ----------------------- Member Function -------------------------
 */

inline int Socket::socketInit(void) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == SOCK_CLOSED)
    throw Socket::InitException();

  return sock;
}

inline void Socket::socketaddrInit(const std::string& host, int port, sock& in) {
  if (!memset(&in, 0, sizeof(in)))
    throw Socket::InitException();
  in.sin_family = AF_INET;
  inet_pton(AF_INET, host.c_str(), &(in.sin_addr));
  in.sin_port = htons(port);
}

inline void Socket::socketOpen(int servFd, sock& in) {
  if (bind(servFd, (struct sockaddr*)&in, sizeof(in)) == -1)
    throw Socket::BindException();
  if (listen(servFd, 256) == -1)
    throw Socket::ListenException();
}

inline void Socket::fdSetInit(fd_set& fs, int fd) {
  FD_ZERO(&fs);
  FD_SET(fd, &fs);
}

void Socket::run(void) {
  struct timeval t;
  t.tv_sec = 1;
  t.tv_usec = 0;

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
  FD_SET(fd, &this->reads);
  if (this->getFdMax() < fd)
    this->setFdMax(fd);

  return fd;
}


void Socket::receiveData(int fd) {
  char buf[BUF_SIZE + 1];
  int recvSize;

  recvSize = recv(fd, buf, BUF_SIZE, 0);
  if (recvSize == -1)
    return ;
  buf[recvSize] = 0;
  this->receivedData[fd] += buf;
  // FIXME: 임시 조건
  if (recvSize < BUF_SIZE) {
    shutdown(fd, SHUT_RD);
    FD_CLR(fd, &reads);

    //std::cout << this->receivedData[fd] << std::endl;
    // HttpRequest request(this->data[fd]);
    //
    // HttpDataFetcher fetcher(request);
    //
    // HttpResponseBuilder rb;
    // HttpResponse response = rb.setRequest(request).setFetcher(fecther).build();
    //
    // std::string ret = response.toString();
    // sendData(fd);

    //sendData(fd);
    // 파싱하고 지지고 볶고, 데이터 전송

    std::string s = util::appendDataLength(this->receivedData[fd].c_str());
    write(this->getWriteFd(), s.c_str(), strlen(s.c_str()));

    std::string ret;
    int receivedDataLength = 0;
    while (1) {
      std::cout << "read before" << std::endl;
      recvSize = read(this->getReadFd(), buf, BUF_SIZE);
      std::cout << "recvSize : " << recvSize << std::endl;
      if (recvSize < 0)
        break;
      buf[recvSize] = 0;
      if (receivedDataLength == 0) {
        std::string b = buf;
        std::pair<int, std::string> data = util::splitSize(b);
        receivedDataLength = data.first;
        ret = data.second;
      }
      else {
        ret += buf;
        // 제대로 되야하는 조건
//        if (receivedDataLength == ret.length())
//          break;
        // FIXME: 임시조건
        if (recvSize < BUF_SIZE)
          break;
      }
    }
    // 왜 0이지 tㅣ팔?h
    std::cout << ret << std::endl;
    std::cout << "\n\n\n\n" << std::endl;
    std::cout << this->receivedData[fd] << std::endl;
    sendData(fd);

    this->receivedData[fd] = "";
  }
}

void Socket::sendData(int fd) {
  FD_SET(fd, &this->writes);
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
  FD_CLR(fd, &this->writes);
  close(fd);
}

void Socket::handShake(int fd) {
  if (fd == this->getServFd())
    acceptConnect();
  else
    receiveData(fd);
}

const char* Socket::InitException::what() const throw() {
  return "Socket init failed";
}

const char* Socket::BindException::what() const throw() {
  return "Socket bind failed";
}

const char* Socket::ListenException::what() const throw() {
  return "Socket listen failed";
}
/*
 * ---------------------- Non-Member Function ----------------------
 */
