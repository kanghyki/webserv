#include "Socket.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Socket::Socket(const std::string& host, const int port) : host(host), port(port), \
                                                            sockFd(SOCK_CLOSED), fdMax(FD_CLOSED) {
  this->sockFd = socketInit();
  socketaddrInit(host, port, this->in);
  socketOpen(this->sockFd, this->in);
  fdSetInit(this->reads, this->sockFd);
  this->fdMax = this->sockFd;
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

const int Socket::getSocketFd(void) const {
  return this->sockFd;
}

const int Socket::getFdMax(void) const {
  return this->fdMax;
}

fd_set Socket::getReads(void) const {
  return this->reads;
}

fd_set Socket::getWrites(void) const {
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

void Socket::socketOpen(int sockFd, sock& in) {
  if (bind(sockFd, (struct sockaddr*)&in, sizeof(in)) == -1)
    throw std::runtime_error("Socket bind failed");
  if (listen(sockFd, 128) == -1)
    throw std::runtime_error("Socket listen failed");
}

void Socket::fdSetInit(fd_set& fs, int fd) {
  FD_ZERO(&fs);
  FD_SET(fd, &fs);
}

void Socket::socketRun(void) {
  while (1) {
    fd_set readsCpy = this->getReads();
    fd_set writesCpy = this->getWrites();

    if (select(this->getFdMax() + 1, &readsCpy, &writesCpy, 0, 0) == -1)
      break;

    for (int i = 0; i < this->getFdMax() + 1; i++) {
      int clntFd = -1;
      if (FD_ISSET(i, &readsCpy)) {
        if (i == this->getSocketFd())
          clntFd = AcceptConnect();
        else
          SendData(i, clntFd);
      }
      if (FD_ISSET(i, &writesCpy))
        CloseSocket(i, clntFd);
    }
  }

  close(this->getSocketFd());
}

int Socket::acceptConnect() {
  int clnt_sock = accept(this->getSocketFd(), 0, 0);
  printf("connected client: %d \n", clnt_sock);

  fcntl(clnt_sock, F_SETFL, O_NONBLOCK);
  FD_SET(clnt_sock, &socket.GetReads());
  this->request_data = RecvData(clnt_sock);
  Http http(this->request_data);
  if (socket.GetFdMax() < clnt_sock)
    socket.SetFdMax(clnt_sock);
  return clnt_sock;
}

/*
 * ---------------------- Non-Member Function ----------------------
 */
