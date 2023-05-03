#include "Server.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Server::Server(Config& config) :
  recvTable(MANAGE_FD_MAX),
  fdMax(FD_CLOSED),
  config(config),
  sessionManager() {
    FD_ZERO(&this->reads);
    FD_ZERO(&this->writes);
    FD_ZERO(&this->listens);

    std::vector<ServerConfig> servers = this->config.getServerConfig();
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

  log::info << "Listening... \"http://" << inet_ntoa(in.sin_addr) << ":" << ntohs(in.sin_port) << "\"" << log::endl;
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

//    std::vector<int> timeout_fd_list = this->connection.getTimeoutList();
//    for (size_t i = 0; i < timeout_fd_list.size(); ++i) {
//      log::cout << INFO << "Client(" << timeout_fd_list[i] << ") timeout, closed\n";
//
//      FD_CLR(timeout_fd_list[i], &this->getReads());
//      FD_CLR(timeout_fd_list[i], &this->getWrites());
//      close(timeout_fd_list[i]);
//      clearReceived(timeout_fd_list[i]);
//      this->connection.remove(timeout_fd_list[i]);
//    }

    for (int i = 0; i < this->getFdMax() + 1; i++) {
      if (FD_ISSET(i, &readsCpy)) {
        if (FD_ISSET(i, &this->listens))
          acceptConnect(i);
        else
          receiveData(i);
      }
      if (FD_ISSET(i, &writesCpy))
        closeSocket(i);

    }
  }

  for (size_t i = 0; i < this->listens_fd.size(); ++i)
    if (close(this->listens_fd[i]) == -1) throw (CloseException());
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


//  if (this->connection.isRegistered(fd) == false)
//    this->connection.add(fd);
  log::info << "Accept " << inet_ntoa(client_addr.sin_addr) << ", create client(" << client_fd << ")" << log::endl;

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
    clearReceived(fd);
    // FIXME:
    if (close(fd) == -1)
      throw (CloseException());
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
  HttpRequest   req;
  HttpResponse  res;

  FD_CLR(fd, &this->getReads());
  log::debug << "this->data[" << fd << "]\n" << this->recvTable[fd].data << log::endl;
  
  try {
    req.parse(getData(fd));
    log::debug << "request good!" << log::endl;
    req.setConfig(this->config.findServerConfig(req.getField("Host")));
    log::debug << "config good!" << log::endl;
    clearReceived(fd);
    log::info << "Request from " << fd << ", Method=\"" << req.getMethod() << "\" URI=\"" << req.getPath() << "\"" << log::endl;
    if (req.isCGI())
      res = Http::executeCGI(req);
    else
      res = Http::processing(req);
  } catch (HttpStatus status) {
    log::debug << "oh error!" << log::endl;
    res = Http::getErrorPage(status, req.getServerConfig());
  }

  log::info << "Response to " << fd <<  ", Status=" << res.getStatusCode() << log::endl;
  sendData(fd, res.toString());
}

void Server::sendData(int fd, const std::string& data) {
  FD_SET(fd, &this->writes);
  if (send(fd, data.c_str(), data.length(), 0) == SOCK_ERROR)
    log::warning << "send failed" << log::endl;
}

void Server::closeSocket(int fd) {
  FD_CLR(fd, &this->getWrites());
  // FIXME:
  if (close(fd) == -1)
    throw (CloseException());
  clearReceived(fd);
//  this->connection.remove(fd);
  log::info << "Closed client(" << fd << ")" << log::endl;
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

size_t Server::getHeaderPos(int fd) const {
  return this->recvTable[fd].headerPos;
}

void Server::setHeaderPos(int fd, size_t pos) {
  this->recvTable[fd].headerPos = pos;
}

void Server::clearReceived(int fd) {
  this->recvTable[fd].data.clear();
  this->recvTable[fd].contentLength = -1;
  this->recvTable[fd].headerPos = 0;
  this->recvTable[fd].status = HEADER_NOT_RECV;
}

int Server::getStatus(int fd) const {
  return this->recvTable[fd].status;
}

void Server::setStatus(int fd, recvStatus status) {
  this->recvTable[fd].status = status;
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

const char* Server::CloseException::what() const throw() {
  return "Server close failed";
}

const char* Server::RecvException::what() const throw() {
  return "Server recv failed";
}

/*
 * ---------------------- Non-Member Function ----------------------
 */
