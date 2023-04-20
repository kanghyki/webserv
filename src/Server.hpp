#ifndef SERVER_HPP
# define SERVER_HPP

# include "./config/Config.hpp"
# include "./network/Socket.hpp"

# include <iostream>
# include <cstdlib>
# include <vector>
# include <signal.h>
# include <unistd.h>


# define READ 0
# define WRITE 1

class Server {
  public:
    Server(Config config);
    void socketInit(ServerConfig config);
    void run(void);

    fd_set getReads(void);
    fd_set getWrites(void);
    const int getFdMax(void) const;
    void setFdMax(int fdMax);

  private:
    Config config;
    std::vector<Socket> socketList;
    std::vector<pid_t> pidList;

    fd_set reads;
    fd_set writes;
    int fdMax;
};

#endif
