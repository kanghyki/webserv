#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <string>
# include <map>
# include <vector>
# include <time.h>

class Connection {
  public:
    Connection();
    ~Connection(void);

    std::vector<int>          getTimeoutList();
    void                      add(int fd);
    void                      update(int fd, unsigned timeout);
    void                      remove(int fd);
    bool                      isRegistered(int fd);

  private:
    static const unsigned int DEFAULT_TIMEOUT;

    // fd, time(sec)
    std::map<int, time_t>     table;
};

#endif
