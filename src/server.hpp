#ifndef SERVER_HPP
#define SERVER_HPP

#define BUF_SIZE (100)

#include <fcntl.h>
#include "socket.hpp"

class Server {
  public:
    Server(std::string host, int port);
    void start(void);
  private:
    Socket socket;
};

#endif
