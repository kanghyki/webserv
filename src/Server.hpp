#ifndef SERVER_HPP
# define SERVER_HPP

# include "./SessionManager.hpp"
# include "./config/ServerConfig.hpp"
# include "./Util.hpp"
# include "./http/HttpRequest.hpp"
# include "./http/HttpResponse.hpp"
# include "./http/Http.hpp"
# include "./http/HttpStatus.hpp"

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

    const int getFdMax(void) const;
    void setFdMax(int fdMax);

    void run();

  private:
    static const int SOCK_CLOSED = -1;
    static const int SOCK_ERROR = -1;
    static const int FD_CLOSED = -1;
    static const int BUF_SIZE = 1024;
    static const int MANAGE_FD_MAX = 1024;
//    static const int HEADER_NOT_RECV = -1;
//    static const int HEADER_RECV = 0;

    struct received {
      std::string data;
      int         contentLength;
      size_t      headerPos;
      int         status;
    };

    enum recvStatus {
      HEADER_NOT_RECV,
      HEADER_RECV,
      BODY_RECV
    };

    std::vector<struct received> recvTable;

//    std::vector<std::string> data;
//    std::vector<int> contentLengths;
//    std::vector<size_t> headerPos;

    const std::string host;
    const int port;
    int servFd;
    int fdMax;
    fd_set reads;
    fd_set writes;
    sock in;

    const int getServFd(void) const;
    fd_set& getReads(void);
    fd_set& getWrites(void);

    inline int socketInit(void);
    inline void socketaddrInit(const std::string& host, int port, sock& in);
    inline void socketOpen(int servFd, sock& in);
    inline void fdSetInit(fd_set& fs, int fd);


    int acceptConnect();
    void receiveData(int fd);
    bool checkContentLength(int fd);
    void sendData(int fd, const std::string& data);
    void closeSocket(int fd);
    void receiveDone(int fd);

    const std::string getData(int fd) const;
    const int         getContentLength(int fd) const;
    const size_t      getHeaderPos(int fd) const;
    const int         getStatus(int fd) const;

    void              addData(int fd, const std::string& data);
    void              setContentLength(int fd, int len);
    void              setHeaderPos(int fd, size_t pos);
    void              setStatus(int fd, recvStatus status);

    void              clearData(int fd);
    void              clearContentLength(int fd);
    void              clearHeaderPos(int fd);
    void              clearReceived(int fd);
    void              clearStatus(int fd);

    void              recvHeader(int fd);
    int               parseContentLength(int fd, size_t start);
    bool              bodyRecvDone(int fd);

    std::map<int, time_t> timeout;
    const std::map<int, time_t>& getTimeRecord() const;
    void removeData(int fd);
    void removeTimeRecord(int fd);
    bool existsTimeRecord(int fd);
    void appendTimeRecord(int fd);
    void DisconnectTimeoutClient();

    ServerConfig config;
    SessionManager sessionManager;

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
