#include "ServerConfig.hpp"

ServerConfig::ServerConfig(): port(4242), host("127.0.0.1"), serverName(""), locations() {}

ServerConfig::~ServerConfig() {}

ServerConfig::ServerConfig(const ServerConfig &obj):
  CommonConfig(obj),
  port(obj.getPort()),
  host(obj.getHost()),
serverName(obj.getServerName()), locations(obj.getLocationConfig()) {}

ServerConfig &ServerConfig::operator=(const ServerConfig &obj) {
  if (this != &obj) {
    this->clientBodySize = obj.getClientBodySize();
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
#include <iostream>

void ServerConfig::addLocationConfig(LocationConfig location) {
  locations.push_back(location);
}
