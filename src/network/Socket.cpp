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
  this->config = config;
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
    throw Socket::InitException();

  return sock;
}

inline void Socket::socketaddrInit(const std::string& host, int port, sock& in) {
  (void)host;

  if (!memset(&in, 0, sizeof(in)))
    throw Socket::InitException();
  in.sin_family = AF_INET;
  in.sin_addr.s_addr = htonl(INADDR_ANY);
  in.sin_port = htons(port);
}

inline void Socket::socketOpen(int servFd, sock& in) {
  if (bind(servFd, (struct sockaddr*)&in, sizeof(in)) == -1)
    throw Socket::BindException();
  if (listen(servFd, 128) == -1)
    throw Socket::ListenException();
}

inline void Socket::fdSetInit(fd_set& fs, int fd) {
  FD_ZERO(&fs);
  FD_SET(fd, &fs);
}

void Socket::socketRun(void) {
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

#include "../Worker.hpp"

void Socket::sendData(int fd) {
  int READ = 0;
  int WRITE = 1;
  static bool is_on = false;
  static int fd1[2], fd2[2];
  FD_SET(fd, &this->getWrites());
//  std::string data("HTTP/1.1 200 OK\r\n\
//      Content-Length: 2048\r\n\
//      Content-Type: text/html\r\n\r\n");


  if (is_on == false) {
    pipe(fd1);
    pipe(fd2);
    int pid = fork();
    if (pid == 0) {
      close(fd1[WRITE]);
      close(fd2[READ]);
      Worker worker(this->config, fd1[READ], fd2[WRITE]);
      worker.routine();
      exit(1);
    }

    close(fd1[READ]);
    close(fd2[WRITE]);
    is_on = true;
  }

  write(fd1[WRITE], this->data[fd].c_str(), this->data[fd].length());

  int zzzzzzz = 10000;
  char buf[zzzzzzz + 1];
  int recv_size;
  int DONE = 0;

  recv_size = read(fd2[READ], buf, zzzzzzz);
  buf[recv_size] = 0;
  std::string data(buf);
  std::cout << data << std::endl;


//  HttpDataFecther h;
//
//  std::string data;
//  std::cout << this->config.getLocationConfig().size() << std::endl;
//  try {
//    for (int i = 0; i < this->config.getLocationConfig().size(); ++i) {
//      if ("." + this->config.getLocationConfig()[i].getPath() == request.getPath()) {
//        if (this->config.getLocationConfig()[i].isAutoIndex()) {
//          data = h.readDirectory(request.getPath());
//        }
//        else {
//          data = h.readFile(request.getPath());
//        }
//      }
//      else {
//        data = h.readFile(request.getPath());
//      }
//    }
//  } catch (std::exception &e) {
//    data = "HTTP/1.1 404 not-found\r\n\r\n";
//  }
//  data += util::readFile("./html/index.html");
  if (send(fd, data.c_str(), strlen(data.c_str()), 0) == SOCK_ERROR)
    std::cout << "[ERROR] send failed\n";
  else
    std::cout << "[Log] send data\n";
  this->data[fd] = "";
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
