#include "HttpConfig.hpp"

HttpConfig::HttpConfig() {}

HttpConfig::~HttpConfig() {}

HttpConfig::HttpConfig(const HttpConfig& obj): CommonConfig(obj), servers(obj.getServerConfig()) {}

HttpConfig& HttpConfig::operator=(const HttpConfig& obj) {
  if (this != &obj) {
    this->clientBodySize = obj.getClientBodySize();
    this->root = obj.getRoot();
    this->errorPagePath = obj.getErrorPagePath();
    this->errorPageStatus = obj.getErrorPageStatus();
    this->index = obj.getIndex();

    this->servers = obj.getServerConfig();
  }
  return *this;
}

std::vector<ServerConfig> HttpConfig::getServerConfig() const {
  return this->servers;
}

void HttpConfig::addServerConfig(ServerConfig server) {
  servers.push_back(server);
}
