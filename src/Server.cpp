#include "Server.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Server::Server(Config config) : config(config) {

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

#include <unistd.h>
void Server::run() {
  HttpConfig http = config.getHttpConfig()[0];
  std::map<int, int> record;
  // <pid, socketList[???]>

//  socker_lsit = worker_record[pid];
//
//socket_list[i].giveData(asd)

  for (size_t i = 0; i < http.getServerConfig().size(); ++i) {
    socketInit(http.getServerConfig()[i]);
  }
//  for (std::vector<ServerConfig>::const_iterator it = http.getServerConfig().begin(); it != http.getServerConfig().end(); ++it) {
//  }
  if (this->socketList.size() > 1) {
    // multiple thread processing...
  }

  for (int i = 0; i < this->socketList.size(); ++i) {
    int pid;

    pid = fork();
    if (pid < 0) {
      std::cerr << "aaa" << std::endl;
    }
    else if (pid == 0) {
      this->socketList[i].socketRun();
      exit(1);
    }
  }

  int WORKERS = 4; // 워커 프로세스 갯수
  for (int i = 0; i < WORKERS; ++i) {
    // run worker 
  }
  
  // 시그널 캐치하면서 소켓이 ㅇ데이터 주면 파이프 읽음
  while (1) {
    // 리퀘스트 읽고
    

    // 워커 프로세스중 가장 여유로운 프로세스에게 
  }
}

/*
 * ---------------------- Non-Member Function ----------------------
 */
