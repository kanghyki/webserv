#ifndef SERVER_HPP
#define SERVER_HPP

#define BUF_SIZE (100)

#include <fcntl.h>
#include "socket.hpp"

class Server {
  public:
    Server(std::string host, int port);
    void Start(void);
  private:
    Socket socket;
    int AcceptConnect(void);
    void SendData(int fd, int clnt_sock);
    void CloseSocket(int fd, int clnt_sock);
};

#endif
