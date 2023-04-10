#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <cstdlib>
# include <vector>
# include "Socket.hpp"

class Server {
  public:
    Server();
    void socketInit(const std::string& host, int port);
    void run(void);

  private:
    std::vector<Socket> socketList;
};

#endif
