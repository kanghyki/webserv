#include "Server.hpp"
#include "Util.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Server::Server(Config config) : config(config), receivedData(10000), receivedDataLength(10000), fdMax(-1) {
  FD_ZERO(&this->reads);
  FD_ZERO(&this->writes);
}

/*
 * -------------------------- Destructor ---------------------------
 */

/*
 * -------------------------- Operator -----------------------------
 */

/*
 * -------------------------- Getter -------------------------------
 */

fd_set Server::getReads(void) {
  return this->reads;
}

fd_set Server::getWrites(void) {
  return this->writes;
}

const int Server::getFdMax(void) const {
  return this->fdMax;
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

void Server::socketInit(ServerConfig config) {
  this->socketList.push_back(Socket(config));
}

void Server::run() {
  HttpConfig http = config.getHttpConfig()[0];

  for (size_t i = 0; i < http.getServerConfig().size(); ++i)
    socketInit(http.getServerConfig()[i]);

  for (size_t i = 0; i < this->socketList.size(); ++i) {
    try {
      int fd1[2];
      int fd2[2];

      util::ftPipe(fd1);
      util::ftPipe(fd2);
//      std::cout << fd1[READ] << "\n";
//      std::cout << fd1[WRITE] << "\n";
//      std::cout << fd2[READ] << "\n";
//      std::cout << fd2[WRITE] << "\n";
//      std::cout << "\n";
      pid_t pid = util::ftFork();
      if (pid == 0) {
        close(fd1[WRITE]);
        close(fd2[READ]);
        this->socketList[i].setReadFd(fd1[READ]);
        this->socketList[i].setWriteFd(fd2[WRITE]);
        this->socketList[i].run();
      }
      close(fd1[READ]);
      close(fd2[WRITE]);
//      fcntl(fd1[WRITE], F_SETFL, O_NONBLOCK);
      fcntl(fd2[READ], F_SETFL, O_NONBLOCK);
//      FD_SET(fd1[WRITE], &this->writes);
      FD_SET(fd2[READ], &this->reads);
      this->setFdMax(fd2[READ]);
//      this->fdMap.insert(std::pair<int, int>(fd1[READ], fd2[WRITE]));
//      this->fdMap.insert(std::pair<int, int>(fd2[WRITE], fd1[READ]));
      this->fdMap.insert(std::pair<int, int>(fd2[READ], fd1[WRITE]));
      this->pidList.push_back(pid);
    } catch (util::ForkException& e) {
      for (size_t i = 0; i < this->pidList.size(); ++i)
        kill(this->pidList[i], SIGKILL);
    } catch (std::exception& e) {
      std::cerr << "[Error] " << e.what() << std::endl;
    }
  }
  std::cout << "server fd max : " << this->getFdMax() << std::endl;

  struct timeval t;
  t.tv_sec = 1;
  t.tv_usec = 0;
  
  while (1) {
    fd_set readsCpy = this->getReads();
    fd_set writesCpy = this->getWrites();

    if (select(this->getFdMax() + 1, &readsCpy, &writesCpy, 0, &t) == -1)
      break;
    std::cout << "server select" << std::endl;

    for (int i = 0; i < this->getFdMax() + 1; i++) {
      if (FD_ISSET(i, &readsCpy))
        receiveData(i);
      if (FD_ISSET(i, &writesCpy)) {
        // 쓰기 버퍼 지우기
        FD_CLR(i, &this->writes);
      }
    }
  }
}

void Server::receiveData(int fd) {
  char buf[BUF_SIZE + 1];
  int recvSize;

  std::cout << "Server recv data\n";
  recvSize = read(fd, buf, BUF_SIZE);
  if (recvSize == -1)
    return ;
  buf[recvSize] = 0;
  if (this->receivedDataLength[fd] == 0) {
    std::string b = buf;
    std::pair<int, std::string> data = util::splitSize(b);
    this->receivedDataLength[fd] = data.first;
    this->receivedData[fd] = data.second;
  }
  else {
    this->receivedData[fd] += buf;
    if (this->receivedDataLength[fd] == this->receivedData[fd].length()) {
      // 워커한테 떤져줘
//      std::cout << "Stop!" << std::endl;
//      sleep(10);
      this->sendData(this->fdMap[fd], this->receivedData[fd]);
      this->receivedData[fd] = "";
      this->receivedDataLength[fd] = 0;
    }
  }
}

void Server::sendData(int fd, std::string data) {
  FD_SET(fd, &this->writes);
  data = util::appendDataLength(data);
  if (write(fd, data.c_str(), strlen(data.c_str())) == SOCK_ERROR)
    std::cout << "[ERROR] send failed\n";
  else
    std::cout << "[Log] send data\n";
}

/*
 * ---------------------- Non-Member Function ----------------------
 */
