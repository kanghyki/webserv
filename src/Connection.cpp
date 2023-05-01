#include "./Connection.hpp"

Connection::Connection(unsigned int timeout_max): timeout_max(timeout_max) {}

Connection::~Connection(void) {}

std::vector<int> Connection::getTimeoutList(void) {
  std::vector<int> timeout_fd_list;

  for (std::map<int, time_t>::iterator it = this->table.begin(); it != this->table.end(); ++it) {
    if (time(NULL) - it->second > this->timeout_max) {
      timeout_fd_list.push_back(it->first);
    }
  }

  return timeout_fd_list;
}

void Connection::add(int fd) {
  this->table.insert(std::make_pair(fd, time(NULL)));
}

void Connection::remove(int fd) {
  this->table.erase(fd);
}

bool Connection::isRegistered(int fd) {
  if (this->table.find(fd) == this->table.end())
    return false;
  return true;
}
