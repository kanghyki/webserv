#include "Config.hpp"

Config::Config() {}

Config::~Config() {}

Config::Config(const Config &obj) : https(obj.getHttpConfig()) {}

Config &Config::operator=(const Config &obj) {
  if (this != &obj)
    this->https = obj.getHttpConfig();
  return *this;
}

std::vector<HttpConfig> Config::getHttpConfig() const {
  return this->https;
}

void Config::addHttpConfig(HttpConfig http) {
  this->https.push_back(http);
}
