#include "server.hpp"
#include <sys/socket.h>

Server::Server(std::string host, int port) : socket(host, port) {
  std::cout << "hello webserv!!" << std::endl;
}

void Server::start(void) {
  fd_set reads_cpy;
  fd_set writes_cpy;
  int clnt_sock;

  while (1) {
    reads_cpy = socket.GetReads();
    writes_cpy = socket.GetWrites();

    if (select(socket.GetFdMax() + 1, &reads_cpy, &writes_cpy, 0, 0) == -1)
      break;

    for (int i = 0; i < socket.GetFdMax() + 1; i++) {
      if (FD_ISSET(i, &reads_cpy)) {
        if (i == socket.GetServSock()) {
          printf("connected client: %d \n", clnt_sock);
          clnt_sock = accept(socket.GetServSock(), 0, 0);
          FD_SET(clnt_sock, &socket.GetReads());
          fcntl(clnt_sock, F_SETFL, O_NONBLOCK);
          if (socket.GetFdMax() < clnt_sock)
            socket.SetFdMax(clnt_sock);
        }
        else {
          FD_CLR(i, &socket.GetReads());
          FD_SET(clnt_sock, &socket.GetWrites());
//          int buf_size = 30;
//          char buf[31];
//          buf[30] = 0;
//          int reads;
//
//          while ((reads = read(clnt_sock, buf, buf_size)) != -1) {
//            printf("%s\n", buf);
//          }
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
      }
      if (FD_ISSET(i, &writes_cpy)) {
        FD_CLR(i, &socket.GetWrites());
        close(clnt_sock);
      }

    }
  }
  close(socket.GetServSock());
}
