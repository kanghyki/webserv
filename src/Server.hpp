#ifndef SERVER_HPP
# define SERVER_HPP

# include "./Connection.hpp"
# include "./SessionManager.hpp"
# include "./Util.hpp"
# include "./config/Config.hpp"
# include "./http/HttpRequest.hpp"
# include "./http/HttpResponse.hpp"
# include "./http/Http.hpp"
# include "./http/HttpStatus.hpp"
# include "./http/HttpHeaderField.hpp"
# include "./Logger.hpp"

# include <arpa/inet.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <time.h>
# include <unistd.h>
# include <vector>

class Server {
  typedef struct sockaddr_in sock;

  public:
    Server(Config& config);
    ~Server(void);

    int getFdMax(void) const;
    void setFdMax(int fdMax);

    void run();

  private:
    static const int SOCK_CLOSED = -1;
    static const int SOCK_ERROR = -1;
    static const int FD_CLOSED = -1;
    static const int BUF_SIZE = 1024;
    static const int MANAGE_FD_MAX = 1024;

    std::vector<HttpRequest> requests;

    enum recvStatus {
      HEADER_NOT_RECV,
      HEADER_RECV,
      BODY_RECV
    };

//    std::vector<std::string> data;
//    std::vector<int> contentLengths;
//    std::vector<size_t> headerPos;

//    int servFd;
    int fdMax;
    fd_set listens;
    fd_set reads;
    fd_set writes;
    std::vector<int> listens_fd;
//    sock in;

//    int getServFd(void) const;
    fd_set& getReads(void);
    fd_set& getWrites(void);

    inline int socketInit(void);
    inline void socketaddrInit(const std::string& host, int port, sock& in);
    inline void socketOpen(int servFd, sock& in);
    inline void fdSetInit(fd_set& fs, int fd);


    int acceptConnect(int server_fd);
    void receiveData(int fd);
    void checkReceiveDone(int fd);
    void sendData(int fd, const std::string& data);
    void closeSocket(int fd);
    void receiveDone(int fd);
    void clearRequest(int fd);

//    const std::string getData(int fd) const;
//    size_t            getContentLength(size_t fd) const;
//    size_t            getHeaderPos(int fd) const;
//    int               getStatus(int fd) const;
//
//    void              addData(int fd, const std::string& data);
//    void              setContentLength(int fd, int len);
//    void              setHeaderPos(int fd, size_t pos);
//    void              setStatus(int fd, recvStatus status);

    void              clearReceived(int fd);
    void              recvHeader(HttpRequest& req);

    const Config&     config;
    Connection        connection;
    SessionManager    sessionManager;

    void              cleanUpTimeout();

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

    class CloseException : public std::exception {
      public:
        const char* what(void) const throw();
    };

    class RecvException : public std::exception {
      public:
        const char* what(void) const throw();
    };
};

#endif
