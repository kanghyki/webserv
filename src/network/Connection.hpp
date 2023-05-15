#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include "../config/Config.hpp"

# include <string>
# include <set>
# include <map>
# include <vector>
# include <time.h>

class Connection {
  public:
    Connection(const Config& config);
    ~Connection(void);

    enum WHAT {
      HEADER = 0,
      BODY,
      SEND
    };

    std::set<int>             getTimeoutList();
    std::set<int>             getGatewayTimeoutList();

    void                      update(int fd, enum WHAT timeout);
    void                      updateKeepAlive(int fd, const ServerConfig& conf);
    void                      updateGateway(int fd);
    int                       updateRequests(int fd, const ServerConfig& conf);

    void                      remove(int fd);
    void                      removeGateway(int fd);
    void                      removeRequests(int fd);

  private:
    const unsigned int        client_header_timeout;
    const unsigned int        client_body_timeout;
    const unsigned int        send_timeout;

    // fd, time(sec)
    std::map<int, time_t>     table;
    std::map<int, time_t>     gateway_table;

    // fd, requests
    std::map<int, int>        req_table;
};

#endif
