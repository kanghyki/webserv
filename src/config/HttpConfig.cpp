#include "HttpConfig.hpp"

HttpConfig::HttpConfig() {}

HttpConfig::~HttpConfig() {}

HttpConfig::HttpConfig(const HttpConfig& obj): CommonConfig(obj), servers(obj.getServerConfig()) {}

HttpConfig& HttpConfig::operator=(const HttpConfig& obj) {
  if (this != &obj) {
    this->clientBodySize = obj.getClientBodySize();
    this->clientBodyTimeout = obj.getClientBodyTimeout();
    this->clientHeaderTimeout = obj.getClientHeaderTimeout();
    this->sendTimeout = obj.getSendTimeout();
    this->root = obj.getRoot();
    this->errorPage = obj.getErrorPage();
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
