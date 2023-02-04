#ifndef SOCKET_HPP
# define SOCKET_HPP

# define SOCK_CLOSED -1
# define FD_CLOSED -1

# include <cstdlib>
# include <cstring>
# include <unistd.h>
# include <iostream>
# include <sys/socket.h>
# include <arpa/inet.h>

class Socket {
  public:
    Socket(std::string host, int port);
    int GetServSock(void) const;
    int GetFdMax(void) const;
    void SetFdMax(int fd_max);
    fd_set& GetReads(void);
  private:
    int serv_sock;
    int fd_max;
	  fd_set reads;
	  struct sockaddr_in serv_addr;
    void SetConfig(std::string host, int port);
    void SocketOpen(void);
};

#endif
