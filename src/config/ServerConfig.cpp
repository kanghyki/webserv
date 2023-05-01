#include "./ServerConfig.hpp"

const int         ServerConfig::DEFAULT_SESSION_TIMEOUT = 600;
const int         ServerConfig::DEFAULT_TIMEOUT = 60;
const short       ServerConfig::DEFAULT_PORT = 80;
const std::string ServerConfig::DEFAULT_HOST = "127.0.0.1";

ServerConfig::ServerConfig():
  CommonConfig(),
  session_timeout(DEFAULT_SESSION_TIMEOUT),
  timeout(DEFAULT_TIMEOUT),
  port(DEFAULT_PORT),
  host(DEFAULT_HOST) {}

ServerConfig::~ServerConfig() {}

ServerConfig::ServerConfig(const ServerConfig& obj):
  CommonConfig(obj),
  session_timeout(obj.getSessionTimeout()),
  timeout(obj.getTimeout()),
  port(obj.getPort()),
  host(obj.getHost()),
  serverName(obj.getServerName()),
  cgi(obj.getCGI()),
  locations(obj.getLocationConfig()) {}

//ServerConfig& ServerConfig::operator=(const ServerConfig& obj) {
//  if (this != &obj) {
//    this->clientBodySize = obj.getClientBodySize();
//    this->session_timeout = obj.getSessionTimeout();
//    this->timeout = obj.getTimeout();
//    this->root = obj.getRoot();
//    this->errorPage = obj.getErrorPage();
//    this->index = obj.getIndex();
//
//    this->port = obj.getPort();
//    this->host = obj.getHost();
//    this->serverName = obj.getServerName();
//    this->cgi = obj.getCGI();
//    this->locations = obj.getLocationConfig();
//  }
//  return *this;
//}

const LocationConfig ServerConfig::findLocationConfig(std::string path) const {
  size_t      pos;
  std::string origin = path;
  bool        end = false;

  while ((pos = path.rfind('/')) != std::string::npos) {

    for (std::vector<LocationConfig>::const_iterator it = this->locations.begin(); it != this->locations.end(); ++it) {
      if (path == it->getPath()) return findLocationConfigRoop(*it, origin.substr(path.length()));
    }

    if (end == true) break;

    path = path.erase(pos);

    if (path.size() == 0) {
      end = true;
      path = "/";
    }
  }

  return LocationConfig(*this);
}

const LocationConfig& ServerConfig::findLocationConfigRoop(const LocationConfig& config, std::string path) const {
  size_t      pos;
  std::string origin = path;
  bool        end = false;

  while ((pos = path.rfind('/')) != std::string::npos) {

    for (std::vector<LocationConfig>::const_iterator it = config.getLocationConfig().begin(); it != config.getLocationConfig().end(); ++it) {
      if (path == it->getPath()) return findLocationConfigRoop(*it, origin.substr(path.length()));
    }

    if (end == true) break;

    path = path.erase(pos);

    if (path.size() == 0) {
      end = true;
      path = "/";
    }
  }

  return config;
}

// getter
//
int ServerConfig::getSessionTimeout() const { return this->session_timeout; }

int ServerConfig::getTimeout() const { return this->timeout; }

short ServerConfig::getPort() const { return this->port; }

std::string ServerConfig::getHost() const { return this->host; }

std::string ServerConfig::getServerName() const { return this->serverName; }

const std::map<std::string, std::string>& ServerConfig::getCGI() const { return this->cgi; }

const std::vector<LocationConfig>& ServerConfig::getLocationConfig() const { return this->locations; }

// setter
//
void ServerConfig::setSessionTimeout(int n) { this->session_timeout = n; }

void ServerConfig::setTimeout(int n) { this->timeout = n; }

void ServerConfig::setPort(short port) { this->port = port; }

void ServerConfig::setHost(std::string host) { this->host = host; }

void ServerConfig::setServerName(std::string serverName) { this->serverName = serverName; }

void ServerConfig::insertCGI(std::string ext, std::string path) { this->cgi.insert(std::make_pair(ext, path)); }

void ServerConfig::addLocationConfig(LocationConfig location) { locations.push_back(location); }
