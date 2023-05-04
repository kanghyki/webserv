#include "./Connection.hpp"
#include "./Logger.hpp"

const unsigned int Connection::DEFAULT_TIMEOUT = 60;

Connection::Connection() {}

Connection::~Connection() {}

std::vector<int> Connection::getTimeoutList(void) {
  std::vector<int> timeout_fd_list;

  for (std::map<int, time_t>::iterator it = this->table.begin(); it != this->table.end(); ++it) {
    if (time(NULL) > it->second) {
      timeout_fd_list.push_back(it->first);
    }
  }

  return timeout_fd_list;
}

void Connection::add(int fd) {
  log::debug << fd << " add timeout " << DEFAULT_TIMEOUT << log::endl;
  this->table.insert(std::make_pair(fd, time(NULL) + DEFAULT_TIMEOUT));
}

void Connection::update(int fd, unsigned int timeout) {
  this->table.erase(fd);
  this->table.insert(std::make_pair(fd, time(NULL) + timeout));
}

void Connection::remove(int fd) {
  log::debug << fd << " erase timeout" << log::endl;
  this->table.erase(fd);
}

bool Connection::isRegistered(int fd) {
  if (this->table.find(fd) == this->table.end())
    return false;
  return true;
}
