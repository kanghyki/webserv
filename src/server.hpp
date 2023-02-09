#ifndef SERVER_HPP
#define SERVER_HPP

#define BUF_SIZE (128)

#include <fcntl.h>
#include "socket.hpp"
#include "file_reader.hpp"

class Server {
  public:
    Server(std::string host, int port);
    void Start(void);
    
  private:
    Socket socket;
    std::string request_data;
    int AcceptConnect();
    void SendData(int fd, int clnt_sock);
    void CloseSocket(int fd, int clnt_sock);
    std::string RecvData(int fd);
};

#endif
