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

    std::vector<int>      getTimeoutList(void);
    void                  add(int fd, unsigned int timeout);
    void                  remove(int fd);
    bool                  isRegistered(int fd);

  private:

    // fd, time(sec)
    std::map<int, time_t> table;
};

#endif
