/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyeongki <hyeongki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 11:11:59 by kanghyki          #+#    #+#             */
/*   Updated: 2023/04/25 14:26:52 by hyeongki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    static const int BUF_SIZE = 1024;
    static const int MANAGE_FD_MAX = 1024;
    static const int TIMEOUT_MAX = 5;

    std::vector<std::string> data;
    std::vector<int> contentLengths;
    std::vector<size_t> headerPos;
    const std::string host;
    const int port;
    int servFd;
    int fdMax;
    fd_set reads;
    fd_set writes;
    sock in;

    const int getServFd(void) const;
    const int getFdMax(void) const;
    fd_set& getReads(void);
    fd_set& getWrites(void);

    void setFdMax(int fdMax);

    inline int socketInit(void);
    inline void socketaddrInit(const std::string& host, int port, sock& in);
    inline void socketOpen(int servFd, sock& in);
    inline void fdSetInit(fd_set& fs, int fd);


    int acceptConnect();
    void receiveData(int fd);
    void checkContentLength(int fd);
    void sendData(int fd);
    void closeSocket(int fd);
    void receiveDone(int fd);

    const std::string getData(int fd) const;
    const int         getContentLength(int fd) const;
    const size_t      getHeaderPos(int fd) const;

    void              addData(int fd, const std::string& data);
    void              setContentLength(int fd, int len);
    void              setHeaderPos(int fd, size_t pos);

    void              clearData(int fd);
    void              clearContentLength(int fd);
    void              clearHeaderPos(int fd);
    void              clearReceived(int fd);

    std::map<int, time_t> timeout;
    const std::map<int, time_t>& getTimeRecord() const;
    void removeData(int fd);
    void removeTimeRecord(int fd);
    bool existsTimeRecord(int fd);
    void appendTimeRecord(int fd);
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
