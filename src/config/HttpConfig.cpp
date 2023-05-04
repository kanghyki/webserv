#include "HttpConfig.hpp"
#include "../Logger.hpp"

HttpConfig::HttpConfig():
  CommonConfig() {}

HttpConfig::~HttpConfig() {}

HttpConfig::HttpConfig(const HttpConfig& obj):
  CommonConfig(obj),
  servers(obj.getServerConfig()) {}

HttpConfig& HttpConfig::operator=(const HttpConfig& obj) {
  if (this != &obj) {
    this->clientBodySize = obj.getClientBodySize();
    this->root = obj.getRoot();
    this->errorPage = obj.getErrorPage();
    this->index = obj.getIndex();

    this->servers = obj.getServerConfig();
  }
  return *this;
}

const std::vector<ServerConfig>& HttpConfig::getServerConfig() const {
  return this->servers;
}

void HttpConfig::addServerConfig(ServerConfig server) {
  servers.push_back(server);
}

const ServerConfig& HttpConfig::findServerConfig(std::string reqHost) const {
  size_t pos;

  if ((pos = reqHost.find(":")) != std::string::npos)
    reqHost = reqHost.substr(0, pos);
  log::debug << "reqHost: " << reqHost << log::endl;

  const std::vector<ServerConfig>& servers = getServerConfig();
  for (std::vector<ServerConfig>::const_iterator sit = servers.begin(); sit != servers.end(); ++sit) {
    log::debug << "serverConfig's server_name: " << sit->getServerName() << log::endl;
    if (sit->getServerName() == reqHost) {
      log::debug << "Matched!" << log::endl;
      return *sit;
    }
  }
  log::debug << "Default server config!" << log::endl;
  return this->servers[0];
}
