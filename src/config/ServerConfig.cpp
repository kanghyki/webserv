#include "./ServerConfig.hpp"
#include "./HttpConfig.hpp"

const short       ServerConfig::DEFAULT_PORT = 80;
const std::string ServerConfig::DEFAULT_HOST = "127.0.0.1";

ServerConfig::ServerConfig(): port(DEFAULT_PORT), host(DEFAULT_HOST) {}

ServerConfig::ServerConfig(const HttpConfig& config): port(DEFAULT_PORT), host(DEFAULT_HOST) {
  this->clientBodySize = config.getClientBodySize();
  this->root = config.getRoot();
}

ServerConfig::~ServerConfig() {}

ServerConfig::ServerConfig(const ServerConfig& obj):
  CommonConfig(obj),
  port(obj.getPort()),
  host(obj.getHost()),
  serverName(obj.getServerName()),
  locations(obj.getLocationConfig()) {}

ServerConfig& ServerConfig::operator=(const ServerConfig& obj) {
  if (this != &obj) {
    this->clientBodySize = obj.getClientBodySize();
    this->clientBodyTimeout = obj.getClientBodyTimeout();
    this->clientHeaderTimeout = obj.getClientHeaderTimeout();
    this->sendTimeout = obj.getSendTimeout();
    this->root = obj.getRoot();
    this->errorPage = obj.getErrorPage();
    this->index = obj.getIndex();

    this->port = obj.getPort();
    this->host = obj.getHost();
    this->serverName = obj.getServerName();
    this->locations = obj.getLocationConfig();
  }
  return *this;
}

// getter

short ServerConfig::getPort() const {
  return this->port;
}

std::string ServerConfig::getHost() const {
  return this->host;
}

std::string ServerConfig::getServerName() const {
  return this->serverName;
}
std::vector<LocationConfig> ServerConfig::getLocationConfig() const {
  return this->locations;
}

// setter

void ServerConfig::setPort(short port) {
  this->port = port;
}

void ServerConfig::setHost(std::string host) {
  this->host = host;
}


void ServerConfig::setServerName(std::string serverName) {
  this->serverName = serverName;
}

void ServerConfig::addLocationConfig(LocationConfig location) {
  locations.push_back(location);
}
