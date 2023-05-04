#include "./Config.hpp"

Config::Config(): is_already_set(false) {}

Config::~Config() {}

Config::Config(const Config& obj) : http(obj.getHttpConfig()) {}

Config& Config::operator=(const Config& obj) {
  if (this != &obj)
    this->http = obj.getHttpConfig();
  return *this;
}

HttpConfig Config::getHttpConfig() const {
  return this->http;
}

void Config::setHttpConfig(HttpConfig http) throw (std::runtime_error) {
  if (this->is_already_set == false) {
    this->is_already_set = true;
    this->http = http;
  }
  else
    throw (std::runtime_error("Multiple http blocks are not allowed"));
}
