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
    std::vector<std::string> recvs;

    int fdMax;
    fd_set listens;
    fd_set reads;
    fd_set writes;
    std::vector<int> listens_fd;

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
    void receiveDone(int fd);

    void              recvHeader(int fd, HttpRequest& req);

    const Config&     config;
    Connection        connection;
    SessionManager    sessionManager;

    void              cleanUpConnection();

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
