#include "./Connection.hpp"

Connection::Connection(unsigned int timeout_max): timeout_max(timeout_max) {}

Connection::~Connection(void) {}

std::vector<int> Connection::getTimeoutList(void) {
  std::vector<int> timeoutList;

  for (std::map<int, time_t>::iterator it = this->table.begin(); it != this->table.end(); ++it) {
    if (time(NULL) - it->second > timeout_max) {
      timeoutList.push_back(it->first);
    }
  }

  return timeoutList;
}

void Connection::add(int fd) {
  this->table.insert(std::make_pair(fd, time(NULL)));
}

void Connection::remove(int fd) {
  this->table.erase(fd);
}

bool Connection::isRegistered(int fd) {
  int value;

  value = this->table[fd];
  if (value == 0)
    return false;
  return true;
}
