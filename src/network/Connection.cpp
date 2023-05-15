#include "./Connection.hpp"

Connection::Connection(const Config& config):
  client_header_timeout(config.getHttpConfig().getClientHeaderTimeout()),
  client_body_timeout(config.getHttpConfig().getClientBodyTimeout()),
  send_timeout(config.getHttpConfig().getSendTimeout()) {
}

Connection::~Connection() {}

std::set<int> Connection::getTimeoutList() {
  std::set<int> timeout_fd_list;

  for (std::map<int, time_t>::iterator it = this->table.begin(); it != this->table.end(); ++it) {
    if (time(NULL) > it->second) {
      timeout_fd_list.insert(it->first);
    }
  }

  return timeout_fd_list;
}

std::set<int> Connection::getGatewayTimeoutList() {
  std::set<int> timeout_fd_list;

  for (std::map<int, time_t>::iterator it = this->gateway_table.begin(); it != this->gateway_table.end(); ++it) {
    if (time(NULL) > it->second) {
      timeout_fd_list.insert(it->first);
    }
  }

  return timeout_fd_list;
}

void Connection::update(int fd, enum WHAT timeout) {
  int to = 60;

  this->table.erase(fd);
  if (timeout == HEADER)
    to = this->client_header_timeout;
  else if (timeout == BODY)
    to = this->client_body_timeout;
  else if (timeout == SEND)
    to = this->send_timeout;

  this->table.insert(std::make_pair(fd, time(NULL) + to));
}

void Connection::updateKeepAlive(int fd, const ServerConfig& conf) {
  if (this->table.find(fd) != this->table.end()) {
    this->table.erase(fd);
  }

  this->table.insert(std::make_pair(fd, time(NULL) + conf.getKeepAliveTimeout()));
}

void Connection::updateGateway(int fd) {
  if (this->gateway_table.find(fd) != this->gateway_table.end()) {
    this->table.erase(fd);
  }

  // TODO: GATEWAT TIMEOUT
  this->gateway_table.insert(std::make_pair(fd, time(NULL) + 30));
}

int Connection::updateRequests(int fd, const ServerConfig& conf) {
  int config_max = conf.getKeepAliveRequests();
  int reqs;

  if (this->req_table.find(fd) != this->req_table.end()) {
    reqs = this->req_table[fd] - 1;
    if (config_max < reqs)
      reqs = config_max;
    this->req_table.erase(fd);
  }
  else
    reqs = conf.getKeepAliveRequests();

  this->req_table.insert(std::make_pair(fd, reqs));

  return reqs;
}

void Connection::remove(int fd) {
  this->table.erase(fd);
}

void Connection::removeGateway(int fd) {
  this->gateway_table.erase(fd);
}

void Connection::removeRequests(int fd) {
  this->req_table.erase(fd);
}
