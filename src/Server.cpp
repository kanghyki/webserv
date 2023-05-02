#include "Server.hpp"
#include "SessionManager.hpp"
#include "Util.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Server::Server(ServerConfig config) :
  recvTable(MANAGE_FD_MAX),
  host(config.getHost()),
  port(config.getPort()),
  servFd(SOCK_CLOSED),
  fdMax(FD_CLOSED),
  config(config),
  connection(config.getTimeout()),
  sessionManager(config.getSessionTimeout()) {
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

Server::~Server(void) {}

/*
 * -------------------------- Operator -----------------------------
 */

/*
 * -------------------------- Getter -------------------------------
 */

int Server::getServFd(void) const {
  return this->servFd;
}

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
  if (sock == SOCK_CLOSED)
    throw Server::InitException();

  return sock;
}

inline void Server::socketaddrInit(const std::string& host, int port, sock& in) {
  if (!memset(&in, 0, sizeof(in)))
    throw Server::InitException();
  in.sin_family = AF_INET;
  inet_pton(AF_INET, host.c_str(), &(in.sin_addr));
  srand(time(0));
  port = rand() % 20000;
  if (port < 2000)
    port += 2000;
  Log::cout() << INFO << "Server launched Host=[" << host << "] Port=[" << port << "]\n\
    \n\
    \n\
    [ http://" << host << ":" << port << " ]\n\
    \n\
    \n";
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

  Log::cout() << INFO << "Server is running\n";
  while (1) {
    fd_set readsCpy = this->getReads();
    fd_set writesCpy = this->getWrites();

    if (select(this->getFdMax() + 1, &readsCpy, &writesCpy, 0, &t) == -1)
      break;

    std::vector<int> timeout_fd_list = this->connection.getTimeoutList();
    for (size_t i = 0; i < timeout_fd_list.size(); ++i) {
      Log::cout() << INFO << "Client(" << timeout_fd_list[i] << ") timeout, closed\n";

      FD_CLR(timeout_fd_list[i], &this->getReads());
      close(timeout_fd_list[i]);
      clearData(timeout_fd_list[i]);
      this->connection.remove(timeout_fd_list[i]);
    }

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
  struct sockaddr_in  client_addr;
  socklen_t           size;

  size = sizeof(client_addr);
  int fd = accept(this->getServFd(), (struct sockaddr *)&client_addr, &size);
  if (fd == -1 || fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
    Log::cout() << WARNING << "connection failed\n";
    return FD_CLOSED;
  }

  FD_SET(fd, &this->getReads());
  if (this->getFdMax() < fd)
    this->setFdMax(fd);


  if (this->connection.isRegistered(fd) == false)
    this->connection.add(fd);
  Log::cout() << INFO << "Accept "<< inet_ntoa(client_addr.sin_addr) << ", create client(" << fd << ")\n";

  return fd;
}

void Server::receiveData(int fd) {
  char buf[BUF_SIZE + 1];
  int recv_size;

  recv_size = recv(fd, buf, BUF_SIZE, 0);
  if (recv_size < 0) {
    Log::cout() << WARNING << "recv failed\n";
    // close socket?
    throw "error";
  }
  else if (recv_size == 0) {
    FD_CLR(fd, &this->reads);
    clearReceived(fd);
    close(fd);
    return ;
  }
  buf[recv_size] = 0;
  addData(fd, buf);
  if (checkContentLength(fd) == true)
    receiveDone(fd);
}

bool Server::checkContentLength(int fd) {
  if (getStatus(fd) == HEADER_NOT_RECV)
    recvHeader(fd);

  if (getStatus(fd) == HEADER_RECV) {
    size_t start = util::toLowerStr(getData(fd)).find("content-length: ");
    if (start != std::string::npos) {
      int len = parseContentLength(fd, start);
      if (len == 0) return true;
      else {
        setStatus(fd, BODY_RECV);
        setContentLength(fd, len);
      }
    }
    else return true;
  }

  if (getStatus(fd) == BODY_RECV) {
    if (bodyRecvDone(fd))
      return true;
  }

  return false;
}

void Server::receiveDone(int fd) {
  HttpResponse res;

  shutdown(fd, SHUT_RD);
  FD_CLR(fd, &this->getReads());
  Log::cout() << DEBUG << "this->data[" << fd << "]\n" << this->recvTable[fd].data;

  try {
    HttpRequest req(getData(fd), this->config);
    clearReceived(fd);
    Log::cout() << INFO << "Request from " << fd << ", Method=\"" << req.getMethod() << "\" URI=\"" << req.getPath() << "\"\n";
    if (req.isCGI())
      res = Http::executeCGI(req, this->sessionManager);
    else
      res = Http::processing(req);
  } catch (HttpStatus status) {
    res = Http::getErrorPage(status, this->config);
  }

  Log::cout() << INFO << "Response to " << fd << ", Status=" << res.getStatusCode() << "\n";
  sendData(fd, res.toString());
}

void Server::sendData(int fd, const std::string& data) {
  FD_SET(fd, &this->writes);
  if (send(fd, data.c_str(), data.length(), 0) == SOCK_ERROR)
    Log::cout() << WARNING << "send failed\n";
}

void Server::closeSocket(int fd) {
  FD_CLR(fd, &this->getWrites());
  close(fd);
  clearReceived(fd);
  this->connection.remove(fd);
}

const std::string Server::getData(int fd) const {
  return this->recvTable[fd].data;
}

size_t Server::getContentLength(size_t fd) const {
  return this->recvTable[fd].contentLength;
}

void Server::addData(int fd, const std::string& data) {
  this->recvTable[fd].data += data;
}

void Server::setContentLength(int fd, int len) {
  this->recvTable[fd].contentLength = len;
}

void Server::clearData(int fd) {
  this->recvTable[fd].data.clear();
}

void Server::clearContentLength(int fd) {
  this->recvTable[fd].contentLength = -1;
}

size_t Server::getHeaderPos(int fd) const {
  return this->recvTable[fd].headerPos;
}

void Server::setHeaderPos(int fd, size_t pos) {
  this->recvTable[fd].headerPos = pos;
}

void Server::clearHeaderPos(int fd) {
  this->recvTable[fd].headerPos = 0;
}

void Server::clearReceived(int fd) {
  clearData(fd);
  clearContentLength(fd);
  clearHeaderPos(fd);
  clearStatus(fd);
}

int Server::getStatus(int fd) const {
  return this->recvTable[fd].status;
}

void Server::setStatus(int fd, recvStatus status) {
  this->recvTable[fd].status = status;
}

void Server::clearStatus(int fd) {
  this->recvTable[fd].status = HEADER_NOT_RECV;
}

void Server::recvHeader(int fd) {
  size_t pos = getData(fd).find("\r\n\r\n");

  if (pos != std::string::npos) {
    setHeaderPos(fd, pos);
    setStatus(fd, HEADER_RECV);
  }
}

int Server::parseContentLength(int fd, size_t start) {
  size_t end = getData(fd).find("\r\n", start);

  return std::atoi(getData(fd).substr(start + 16, end - start + 1).c_str());
}

bool Server::bodyRecvDone(int fd) {
  if (getContentLength(fd) == getData(fd).substr(getHeaderPos(fd) + 4).length())
    return true;
  return false;
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
