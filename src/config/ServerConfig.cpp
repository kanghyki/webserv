#include "ServerConfig.hpp"

ServerConfig::ServerConfig() {}
ServerConfig::~ServerConfig() {}
ServerConfig::ServerConfig(const ServerConfig &obj) {}
ServerConfig &ServerConfig::operator=(const ServerConfig &obj) {
  if (this != &obj) {
  }
  return *this;
}

int ServerConfig::getPort() const {
  return this->port;
}

void ServerConfig::setPort(int port) {
  this->port = port;
}

void ServerConfig::setHost(std::string host) {
  this->host = host;
}

std::string ServerConfig::getHost() const {
  return this->host;
}

std::string ServerConfig::getServerName() const {
  return this->serverName;
}

void ServerConfig::setServerName(std::string serverName) {
  this->serverName = serverName;
}

std::vector<LocationConfig> ServerConfig::getLocationConfig() const {
  return this->locations;
}

void ServerConfig::addLocationConfig(LocationConfig location) {
  locations.push_back(location);
}
