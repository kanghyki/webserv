#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <unistd.h>
# include <sys/socket.h>
# include <arpa/inet.h>

class Socket {
  typedef struct sockaddr_in sock;

  public:
    Socket(const std::string& host, const int port);
    ~Socket(void);

    const int getSocketFd(void) const;
    const int getFdMax(void) const;
    void setFdMax(int fdMax);
    fd_set getReads(void) const;
    fd_set getWrites(void) const;

    void socketRun();

  private:
    static const int SOCK_CLOSED = -1;
    static const int FD_CLOSED = -1;

    const std::string host;
    const int port;
    int sockFd;
    int fdMax;
    fd_set reads;
    fd_set writes;

    sock in;

    int socketInit(void);
    void socketaddrInit(const std::string& host, int port, sock& in);
    void socketOpen(int sockFd, sock& in);
    void fdSetInit(fd_set& fs, int fd);

    int acceptConnect();
};

#endif
