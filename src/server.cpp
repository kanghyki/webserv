#include "server.hpp"
#include <sys/socket.h>

Server::Server(std::string host, int port) : socket(host, port) {
  std::cout << "hello webserv!!" << std::endl;
}

void Server::Start(void) {
  fd_set reads_cpy;
  fd_set writes_cpy;
  int clnt_sock;

  while (1) {
    reads_cpy = socket.GetReads();
    writes_cpy = socket.GetWrites();

    printf("select before\n");
    if (select(socket.GetFdMax() + 1, &reads_cpy, &writes_cpy, 0, 0) == -1)
      break;
    printf("select after\n");

    for (int i = 0; i < socket.GetFdMax() + 1; i++) {
      if (FD_ISSET(i, &reads_cpy)) {
        if (i == socket.GetServSock())
          clnt_sock = AcceptConnect();
        else
          SendData(i, clnt_sock);
      }
      if (FD_ISSET(i, &writes_cpy))
        CloseSocket(i, clnt_sock);
    }
  }
  close(socket.GetServSock());
}

int Server::AcceptConnect() {
  int clnt_sock = accept(socket.GetServSock(), 0, 0);
  printf("connected client: %d \n", clnt_sock);
  fcntl(clnt_sock, F_SETFL, O_NONBLOCK);
  FD_SET(clnt_sock, &socket.GetReads());
  if (socket.GetFdMax() < clnt_sock)
    socket.SetFdMax(clnt_sock);
  return clnt_sock;
}

void Server::SendData(int fd, int clnt_sock) {
  printf("send data\n");
  FD_CLR(fd, &socket.GetReads());
  FD_SET(clnt_sock, &socket.GetWrites());
  const char * html = "\
HTTP/1.1 200 OK\r\n\
Server: Hyeongki&Kanghyki server\r\n\
Content-Length: 63\r\n\
Content-Type: text/html\r\n\
\r\n\
<!doctype html>\
<html>\
<body>\
<h1>Hello, World!</h1>\
</body>\
</html>";
  send(clnt_sock, html, strlen(html), 0);
}

void Server::CloseSocket(int fd, int clnt_sock) {
  printf("close\n");
  FD_CLR(fd, &socket.GetWrites());
  close(clnt_sock);
}
