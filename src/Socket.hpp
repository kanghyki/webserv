#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <fcntl.h>
# include <unistd.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <vector>

# include "Except.hpp"

class Socket {
  typedef struct sockaddr_in sock;

  public:
    Socket(const std::string& host, const int port);
    ~Socket(void);

    const int getServFd(void) const;
    const int getFdMax(void) const;
    void setFdMax(int fdMax);
    fd_set& getReads(void);
    fd_set& getWrites(void);

    void socketRun();

  private:
    static const int SOCK_CLOSED = -1;
    static const int SOCK_ERROR = -1;
    static const int FD_CLOSED = -1;
    static const int BUF_SIZE = 128;

    std::vector<std::string> data;
    const std::string host;
    const int port;
    int servFd;
    int fdMax;
    fd_set reads;
    fd_set writes;

    sock in;

    inline int socketInit(void);
    inline void socketaddrInit(const std::string& host, int port, sock& in);
    inline void socketOpen(int servFd, sock& in);
    inline void fdSetInit(fd_set& fs, int fd);

    int acceptConnect();
    void receiveData(int fd);
    void sendData(int fd);
    void closeSocket(int fd);
    void handShake(int fd);
};

#endif
