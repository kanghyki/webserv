#include "HttpConfig.hpp"

HttpConfig::HttpConfig() {}
HttpConfig::~HttpConfig() {}
HttpConfig::HttpConfig(const HttpConfig &obj) {}
HttpConfig &HttpConfig::operator=(const HttpConfig &obj) {
  if (this != &obj) {
  }
  return *this;
}

std::vector<ServerConfig> HttpConfig::getServerConfig() const {
  return this->servers;
}

void HttpConfig::addServerConfig(ServerConfig server) {
  servers.push_back(server);
}
