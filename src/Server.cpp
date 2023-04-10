#include "Server.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Server::Server() {}

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

void Server::socketInit(const std::string& host, int port) {
  this->socketList.push_back(Socket(host, port));
}

void Server::run() {
  if (this->socketList.size() > 1) {
    // multiple thread processing...
  }
  this->socketList[0].socketRun();
}

/*
 * ---------------------- Non-Member Function ----------------------
 */
