#include "HttpConfig.hpp"

const int HttpConfig::DEFAULT_CLIENT_HEADER_TIMEOUT = 60;
const int HttpConfig::DEFAULT_CLIENT_BODY_TIMEOUT = 60;
const int HttpConfig::DEFAULT_SEND_TIMEOUT = 60;

HttpConfig::HttpConfig():
  CommonConfig(),
  client_header_timeout(DEFAULT_CLIENT_HEADER_TIMEOUT),
  client_body_timeout(DEFAULT_CLIENT_BODY_TIMEOUT),
  send_timeout(DEFAULT_SEND_TIMEOUT) {}

HttpConfig::~HttpConfig() {}

HttpConfig::HttpConfig(const HttpConfig& obj):
  CommonConfig(obj),
  client_header_timeout(obj.getClientHeaderTimeout()),
  client_body_timeout(obj.getClientBodyTimeout()),
  send_timeout(obj.getSendTimeout()),
  servers(obj.getServerConfig()) {}

HttpConfig& HttpConfig::operator=(const HttpConfig& obj) {
  if (this != &obj) {
    this->clientMaxBodySize = obj.getClientMaxBodySize();
    this->root = obj.getRoot();
    this->errorPage = obj.getErrorPage();
    this->index = obj.getIndex();

    this->client_header_timeout = obj.getClientHeaderTimeout();
    this->client_body_timeout = obj.getClientBodyTimeout();
    this->send_timeout = obj.getSendTimeout();
    this->servers = obj.getServerConfig();
  }
  return *this;
}

// getter

int HttpConfig::getClientHeaderTimeout() const { return this->client_header_timeout; }

int HttpConfig::getClientBodyTimeout() const { return this->client_body_timeout; }

int HttpConfig::getSendTimeout() const { return this->send_timeout; }

const std::vector<ServerConfig>& HttpConfig::getServerConfig() const {
  return this->servers;
}

// setter

void HttpConfig::setClientHeaderTimeout(int n) { this->client_header_timeout = n; }

void HttpConfig::setClientBodyTimeout(int n) { this->client_body_timeout = n; }

void HttpConfig::setSendTimeout(int n) { this->send_timeout = n; }

void HttpConfig::addServerConfig(ServerConfig server) {
  for (size_t i = 0; i < this->servers.size(); ++i) {
    if (this->servers[i].getPort() == server.getPort())
      throw std::runtime_error("Server blocks on the same port are not allowed");
    if (this->servers[i].getServerName() == server.getServerName())
      throw std::runtime_error("Server blocks on the same server_name are not allowed");
  }
  this->servers.push_back(server);
}

//

const ServerConfig& HttpConfig::findServerConfig(std::string reqHost) const {
  size_t pos;

  if ((pos = reqHost.find(":")) != std::string::npos)
    reqHost = reqHost.substr(0, pos);

  const std::vector<ServerConfig>& servers = getServerConfig();
  for (std::vector<ServerConfig>::const_iterator sit = servers.begin(); sit != servers.end(); ++sit) {
    if (sit->getServerName() == reqHost) {
      return *sit;
    }
  }
  return this->servers[0];
}
