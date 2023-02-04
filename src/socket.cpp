#include "socket.hpp"

Socket::Socket(std::string host, int port) : serv_sock(SOCK_CLOSED),
                                             fd_max(FD_CLOSED){
	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (serv_sock == SOCK_CLOSED)
    throw std::runtime_error("Socket init failed");
  SetConfig(host, port);
  SocketOpen();
}

void Socket::SetConfig(std::string host, int port) {
  (void)host;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
}

void Socket::SocketOpen() {
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, 
                             sizeof(serv_addr)) == -1)
    throw std::runtime_error("Socket bind failed");
	if (listen(serv_sock, 16) == -1)
    throw std::runtime_error("Socket listen failed");
	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	fd_max = serv_sock;
}

int Socket::GetServSock(void) const {
  return this->serv_sock;
}

int Socket::GetFdMax(void) const {
  return this->fd_max;
}

void Socket::SetFdMax(int fd_max) {
  this->fd_max = fd_max;
}

fd_set& Socket::GetReads(void) {
  return this->reads;
}
