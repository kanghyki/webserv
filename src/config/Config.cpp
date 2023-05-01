#include "./Config.hpp"
#include "../http/HttpStatus.hpp"

Config::Config() {}

Config::~Config() {}

Config::Config(const Config& obj) : servers(obj.getServerConfig()) {}

const std::vector<ServerConfig>& Config::getServerConfig() const {
  return this->servers;
}

void Config::addServerConfig(ServerConfig server) {
  servers.push_back(server);
}

const ServerConfig& Config::findServerConfig(std::string reqHost) const {
  size_t pos;

  if ((pos = reqHost.find(":")) != std::string::npos)
    reqHost = reqHost.substr(pos + 1);

  std::vector<ServerConfig> servers = getServerConfig();
  for (std::vector<ServerConfig>::iterator sit = servers.begin(); sit != servers.end(); ++sit) {
    if (sit->getServerName() == reqHost) {
      return *sit;
    }
  }
  return getServerConfig()[0];
}
