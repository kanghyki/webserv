#include "Config.hpp"

Config::Config() {}
Config::~Config() {}
Config::Config(const Config &obj) {}
Config &Config::operator=(const Config &obj) {
  if (this != &obj) {
  }
  return *this;
}

std::vector<HttpConfig> Config::getHttpConfig() const {
  return this->https;
}

void Config::addHttpConfig(HttpConfig http) {
  https.push_back(http);
}
