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

    if (select(socket.GetFdMax() + 1, &reads_cpy, &writes_cpy, 0, 0) == -1)
      break;

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
  this->request_data = RecvData(clnt_sock);
  std::cout << this->request_data << std::endl;
  if (socket.GetFdMax() < clnt_sock)
    socket.SetFdMax(clnt_sock);
  return clnt_sock;
}

void Server::SendData(int fd, int clnt_sock) {
  FileReader fr("./html/index.html");
  std::string content = fr.Read();
  printf("send data\n");
  FD_CLR(fd, &socket.GetReads());
  FD_SET(clnt_sock, &socket.GetWrites());
  std::string protocol("HTTP/1.1 200 OK\r\n");
  std::string server("Server: Hyeongki&Kanghyki server\r\n");
  std::string cnt_len("Content-Length: 2048\r\n");
  std::string cnt_type("Content-Type: text/html\r\n\r\n");
  std::string html = protocol + server + cnt_len + cnt_type + content;
  send(clnt_sock, html.c_str(), strlen(html.c_str()), 0);
}

void Server::CloseSocket(int fd, int clnt_sock) {
  printf("close\n");
  FD_CLR(fd, &socket.GetWrites());
  close(clnt_sock);
}

std::string Server::RecvData(int fd) {
  char buf[BUF_SIZE + 1];
  size_t recv_size;
  std::string ret;

  while (1) {
    recv_size = recv(fd, buf, BUF_SIZE, 0);
    buf[BUF_SIZE] = 0;
    ret += buf;
    if (recv_size < BUF_SIZE)
      break;
  }
  shutdown(fd, SHUT_RD);

  return ret;
}
