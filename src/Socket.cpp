#include "Socket.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Socket::Socket(const std::string& host, const int port) : host(host), port(port), \
                                                            sock(SOCK_CLOSED), fdMax(FD_CLOSED) {
  this->sock = socket(AF_INET, SOCK_STREAM, 0);
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

/*
 * ---------------------- Non-Member Function ----------------------
 */
