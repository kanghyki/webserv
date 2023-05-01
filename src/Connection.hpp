#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <string>
# include <map>
# include <vector>
# include <time.h>

class Connection {
  public:
    Connection(unsigned int timeout_max);
    ~Connection(void);

    std::vector<int>      getTimeoutList(void);
    void                  add(int fd);
    void                  remove(int fd);
    bool                  isRegistered(int fd);

  private:
    const unsigned int    timeout_max;

    // fd, time(sec)
    std::map<int, time_t> table;
};

#endif
