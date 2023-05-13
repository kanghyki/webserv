#ifndef SERVER_HPP
# define SERVER_HPP

# include "./Connection.hpp"
# include "../etc/Logger.hpp"
# include "../etc/Util.hpp"
# include "./SessionManager.hpp"
# include "../config/Config.hpp"
# include "../http/Http.hpp"
# include "../http/HttpRequest.hpp"
# include "../http/HttpResponse.hpp"
# include "../http/HttpStatus.hpp"

# include <arpa/inet.h>
# include <fcntl.h>
# include <time.h>
# include <unistd.h>
# include <vector>
# include <sys/socket.h>

class Server {
  public:
    Server(Config& config);
    ~Server(void);

    void run();

  private:
    static const size_t         BIND_MAX_TRIES;
    static const size_t         LISTEN_MAX_TRIES;
    static const size_t         TRY_SLEEP_TIME;
    static const int            BUF_SIZE;
    static const int            MANAGE_FD_MAX;

    static const std::string    HEADER_DELIMETER;
    static const std::string    CHUNKED_DELIMETER;

    std::vector<int>            listens_fd;
    std::vector<HttpRequest>    requests;
    std::vector<HttpResponse>   responses;
    std::vector<std::string>    recvs;
    std::vector<CGI>            cgis; 

    int                         fdMax;
    fd_set                      listens;
    fd_set                      reads;
    fd_set                      writes;

    const Config&               config;
    Connection                  connection;
    SessionManager              sessionManager;

    inline int  socketInit(void);
    inline void socketaddrInit(const std::string& host, int port, sockaddr_in& in);
    inline void socketOpen(int servFd, sockaddr_in& in);
    inline void fdSetInit(fd_set& fs, int fd);

    void        acceptConnect(int server_fd);
    void        receiveData(int fd);
    void        checkReceiveDone(int fd);
    void        receiveHeader(int fd, HttpRequest& req);
    void        receiveDone(int fd);
    void        cgiDone(CGI* cgi);
    void        addExtraHeader(int fd, HttpRequest& req, HttpResponse& res);
    void        sendData(int fd);
    void        closeConnection(int fd);
    void        keepAliveConnection(int fd);
    void        cleanUpConnection();
    bool        checkCGIFd(int fd);
    CGI*        getCGI(int fd);
    void        executeCGI(const HttpRequest& req, SessionManager& sm, int reqFd);
    void        eraseCGI(std::vector<CGI>& cgis, CGI* cgi);
};

#endif
