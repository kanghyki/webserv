#include "./LocationConfig.hpp"
#include "./ServerConfig.hpp"

const bool        LocationConfig::DEFAULT_AUTOINDEX = false;
const std::string LocationConfig::DEFAULT_ALIAS = "";
const std::string LocationConfig::DEFAULT_PATH = "/";

LocationConfig::LocationConfig():
  CommonConfig(),
  alias(DEFAULT_ALIAS),
  path(DEFAULT_PATH),
  _return(std::make_pair(-1, "")),
  autoindex(DEFAULT_AUTOINDEX) {
    // support method
    this->limitExcept.push_back("GET");
    this->limitExcept.push_back("PUT");
    this->limitExcept.push_back("POST");
    this->limitExcept.push_back("DELETE");
    this->limitExcept.push_back("HEAD");
  }

LocationConfig::LocationConfig(const ServerConfig& config):
  CommonConfig(config),
  alias(DEFAULT_ALIAS),
  path(DEFAULT_PATH),
  _return(std::make_pair(-1, "")),
  autoindex(DEFAULT_AUTOINDEX) {}

LocationConfig::LocationConfig(const LocationConfig& obj):
  CommonConfig(obj),
  alias(obj.getAlias()),
  path(obj.getPath()),
  limitExcept(obj.getLimitExcept()),
  _return(obj.getReturn()),
  autoindex(obj.isAutoindex()),
  locations(obj.getLocationConfig()) {}

LocationConfig::~LocationConfig() {}

LocationConfig& LocationConfig::operator=(const LocationConfig& obj) {
  if (this != &obj) {
    this->clientMaxBodySize = obj.getClientMaxBodySize();
    this->root = obj.getRoot();
    this->errorPage = obj.getErrorPage();
    this->index = obj.getIndex();

    this->alias = obj.getAlias();
    this->path = obj.getPath();
    this->limitExcept = obj.getLimitExcept();
    this->_return = obj.getReturn();
    this->autoindex = obj.isAutoindex();
    this->locations = obj.getLocationConfig();
  }

  return *this;
}

// getter

std::string LocationConfig::getAlias() const { return this->alias; }

std::string LocationConfig::getPath() const { return this->path; }

const std::vector<std::string>& LocationConfig::getLimitExcept() const {
  return this->limitExcept;
}

bool LocationConfig::isMethodAllowed(std::string method) const {
  if (this->limitExcept.size() == 0)
    return true;

  for (size_t i = 0; i < this->limitExcept.size(); ++i) {
    if (this->limitExcept[i] == method)
      return true;
  }

  return false;
}

bool LocationConfig::hasReturn() const {
  if (this->_return.first == -1)
    return false;
  return true;
}

std::pair<int, std::string> LocationConfig::getReturn() const { return this->_return; }

bool LocationConfig::isAutoindex() const { return this->autoindex; }

const std::vector<LocationConfig>& LocationConfig::getLocationConfig() const {
  return this->locations;
}

// setter

void LocationConfig::setAlias(std::string alias) { this->alias = alias; }

void LocationConfig::setPath(std::string path) { this->path = path; }

void LocationConfig::setLimitExcept(std::vector<std::string> methods) { this->limitExcept = methods; }

void LocationConfig::setReturn(int status, std::string path) {
  this->_return = std::make_pair(status, path);
}

void LocationConfig::setAutoindex(bool autoIndex) { this->autoindex = autoIndex; }

void LocationConfig::addLocationConfig(LocationConfig location) { this->locations.push_back(location); }

std::string LocationConfig::toStringLimitExcept() const {
  std::string ret;

  for (size_t i = 0; i < this->limitExcept.size(); ++i) {
    ret += this->limitExcept[i];
    if (i + 1 < this->limitExcept.size())
      ret += ", ";
  }

  return ret;
}
