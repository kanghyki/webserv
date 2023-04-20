#include "Server.hpp"
#include "Util.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Server::Server(Config config) : config(config) {
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

/*
 * -------------------------- Setter -------------------------------
 */

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
      pid_t pid = util::ftFork();
      int fd1[2];
      int fd2[2];

      util::ftPipe(fd1);
      util::ftPipe(fd2);
      if (pid == 0) {
        close(fd1[WRITE]);
        close(fd2[READ]);
        this->socketList[i].setReadFd(fd1[READ]);
        this->socketList[i].setWriteFd(fd2[WRITE]);
        this->socketList[i].run();
      }
      close(fd1[READ]);
      close(fd2[WRITE]);
      FD_SET(fd1[WRITE], &this->reads);
      FD_SET(fd2[READ], &this->writes);
      this->pidList.push_back(pid);
    } catch (util::ForkException& e) {
      for (size_t i = 0; i < this->pidList.size(); ++i)
        kill(this->pidList[i], SIGKILL);
    } catch (std::exception& e) {
      std::cerr << "[Error] " << e.what() << std::endl;
    }
  }
  int status;
  wait(&status);
  
//  while (1) {
//    fd_set readsCpy = this->getReads();
//    fd_set writesCpy = this->getWrites();
//
//    if (select(this->getFdMax() + 1, &readsCpy, &writesCpy, 0, &t) == -1)
//      break;
//
//    for (int i = 0; i < this->getFdMax() + 1; i++) {
//      if (FD_ISSET(i, &readsCpy))
//        handShake(i);
//      if (FD_ISSET(i, &writesCpy))
//        closeSocket(i);
//    }
//  }

}

fd_set Server::getReads(void) {
  return this->reads;
}

fd_set Server::getWrites(void) {
  return this->writes;
}

const int Server::getFdMax(void) const {
  return this->fdMax;
}

void Server::setFdMax(int fdMax) {
  this->fdMax = fdMax;
}

/*
 * ---------------------- Non-Member Function ----------------------
 */
