#ifndef SERVER_HPP
# define SERVER_HPP

# include "./config/Config.hpp"
# include "./network/Socket.hpp"

# include <iostream>
# include <cstdlib>
# include <vector>

class Server {
  public:
    Server(Config config);
    void socketInit(ServerConfig config);
    void run(void);

  private:
    Config config;
    std::vector<Socket> socketList;
};

#endif
