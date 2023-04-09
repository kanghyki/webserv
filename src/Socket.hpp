#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <sys/socket.h>
# include <arpa/inet.h>

class Socket {
  public:
    Socket(const std::string& host, const int port);

  private:
    const int SOCK_CLOSED = -1;
    const int FD_CLOSED = -1;

    const std::string host;
    const int port;
    int sock;
    int fdMax;
};

#endif
