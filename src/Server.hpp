#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include "Socket.hpp"

class Server {
  public:
    Server(const std::string& host, const int port);

  private:
    Socket socket;
};

#endif
