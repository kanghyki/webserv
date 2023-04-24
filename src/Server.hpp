#ifndef SERVER_HPP
# define SERVER_HPP

# include "./config/ServerConfig.hpp"
# include "./Util.hpp"
# include "./http/HttpRequest.hpp"
# include "./http/HttpResponse.hpp"

# include <arpa/inet.h>
# include <fcntl.h>
# include <iostream>
# include <sys/socket.h>
# include <time.h>
# include <unistd.h>
# include <vector>

class Server {
  typedef struct sockaddr_in sock;

  public:
    Server(ServerConfig config);
    ~Server(void);

    void run();

  private:
    static const int SOCK_CLOSED = -1;
    static const int SOCK_ERROR = -1;
    static const int FD_CLOSED = -1;
    static const int BUF_SIZE = 128;
    static const int TIMEOUT_MAX = 5;

    std::vector<std::string> data;
    const std::string host;
    const int port;
    int servFd;
    int fdMax;
    fd_set reads;
    fd_set writes;
    std::map<int, time_t> timeout;
    sock in;

    const int getServFd(void) const;
    const int getFdMax(void) const;
    fd_set& getReads(void);
    fd_set& getWrites(void);
    const std::map<int, time_t>& getTimeRecord() const;

    void setFdMax(int fdMax);

    inline int socketInit(void);
    inline void socketaddrInit(const std::string& host, int port, sock& in);
    inline void socketOpen(int servFd, sock& in);
    inline void fdSetInit(fd_set& fs, int fd);


    int acceptConnect();
    void receiveData(int fd);
    void sendData(int fd, std::string header, std::string body);
    void closeSocket(int fd);
    void receiveDone(int fd, std::string header, std::string body);
    void removeTimeRecord(int fd);
    void DisconnectTimeoutClient();

    ServerConfig config;

  public:
    class InitException : public std::exception {
      public:
        const char* what(void) const throw();
    };

    class BindException : public std::exception {
      public:
        const char* what(void) const throw();
    };

    class ListenException : public std::exception {
      public:
        const char* what(void) const throw();
    };
};

#endif
