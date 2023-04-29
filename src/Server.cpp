#include "Server.hpp"
#include "Util.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Server::Server(ServerConfig config) : recvTable(MANAGE_FD_MAX), host(config.getHost()), port(config.getPort()), \
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
  srand(time(0));
  port = rand() % 20000;
  if (port < 2000)
    port += 2000;
  std::cout << "[ http://localhost:" << port << "/html/index.html ]" << std::endl;
  std::cout << "[ http://localhost:" << port << " ]" << std::endl;
  std::cout << "[ http://localhost:" << port << "/cgi-bin/hello.py ]" << std::endl;
  std::cout << "[ http://localhost:" << port << "/html/upload.html ]" << std::endl;
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

void Server::removeData(int fd) {
  this->recvTable[fd].data.clear();
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

// TODO: MUTIPLE CGI ? RESOURCE RECOVERY
void Server::DisconnectTimeoutClient() {
  const std::map<int, time_t>&  record = getTimeRecord();
  std::vector<int>              removeList;

  for (std::map<int, time_t>::const_iterator it = record.begin(); it != record.end(); ++it) {
    time_t reqTime = it->second;
    time_t curTime;

    time(&curTime);
    if (curTime - reqTime > this->config.getTimeout()) {
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
  if (recv_size < 0)
    throw "error";
  else if (recv_size == 0) {
    std::cout << "recv_size:0" << std::endl;
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
  std::cout << "@---this->data[" << fd << "]" << std::endl;
  std::cout << this->recvTable[fd].data;
  std::cout << "@---" << std::endl;

  try {
    HttpRequest req(getData(fd), this->config, this->session);
    if (req.isCGI()) 
      res = Http::executeCGI(req, this->reads, this->fdMax);
    else
      res = Http::processing(req);
    clearReceived(fd);
  } catch (HttpStatus status) {
    res = Http::getErrorPage(status, this->config);
  }

  sendData(fd, res.toString());
  clearReceived(fd);
}

void Server::sendData(int fd, const std::string& data) {
  FD_SET(fd, &this->writes);
  if (send(fd, data.c_str(), data.length(), 0) == SOCK_ERROR)
    std::cout << "[ERROR] send failed\n";
  else
    std::cout << "[Log] send data\n";
  removeTimeRecord(fd);
}

void Server::closeSocket(int fd) {
  std::cout << "[Log] close\n";
  FD_CLR(fd, &this->getWrites());
  close(fd);
}

const std::string Server::getData(int fd) const {
  return this->recvTable[fd].data;
}

const int Server::getContentLength(int fd) const {
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

const size_t Server::getHeaderPos(int fd) const {
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

const int Server::getStatus(int fd) const {
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
