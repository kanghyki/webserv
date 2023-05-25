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

# include <signal.h>
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
    static const size_t         BUF_SIZE;
    static const size_t         MANAGE_FD_MAX;

    static const std::string    HEADER_DELIMETER;
    static const std::string    CHUNKED_DELIMETER;

    std::vector<int>            listens_fd;
    std::map<int, HttpRequest>  requests;
    std::map<int, HttpResponse> responses;
    std::map<int, std::string>  recvs;

    std::map<int, int>          cgi_map;
    std::map<int, int>          file_map;

    int                         fdMax;
    fd_set                      listens;
    fd_set                      reads;
    fd_set                      writes;

    const Config&               config;
    Connection                  connection;
    SessionManager              sessionManager;

    void                        acceptConnect(int server_fd);
    void                        receiveData(int fd);
    void                        checkReceiveDone(int fd);
    void                        receiveHeader(int fd, HttpRequest& req);
    void                        prepareIO(int client_fd);
    bool                        isReadFd(const HttpRequest& req, const HttpResponse& res);
    void                        postProcessing(int client_fd);
    void                        addExtraHeader(int fd, HttpRequest& req, HttpResponse& res);
    void                        sendData(int fd);

    void                        closeConnection(int fd);
    void                        keepAliveConnection(int fd);

    void                        cleanUpConnection();

    bool                        isCgiPipe(int fd) const;
    void                        writeCGI(int fd);
    void                        readCGI(int fd);

    bool                        isFileFd(int fd) const;
    void                        writeFile(int fd);
    void                        readFile(int fd);

    void                        ft_fd_set(int fd, fd_set& set);
    void                        ft_fd_clr(int fd, fd_set& set);

    int                         init_socket(void);
    void                        init_sockaddr_in(const std::string& host, int port, sockaddr_in& in);
    void                        open_socket(int servFd, sockaddr_in& in);

    void                        setup_server();
    void                        loop();
};

#endif
