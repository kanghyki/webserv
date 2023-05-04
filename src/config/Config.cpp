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

#include "../Logger.hpp"

const ServerConfig& Config::findServerConfig(std::string reqHost) const {
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
