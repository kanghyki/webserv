#include "./LocationConfig.hpp"
#include "./ServerConfig.hpp"

const bool LocationConfig::DEFAULT_AUTOINDEX = false;

LocationConfig::LocationConfig(): autoIndex(DEFAULT_AUTOINDEX) {}

LocationConfig::LocationConfig(const ServerConfig& config): autoIndex(DEFAULT_AUTOINDEX) {
  this->clientBodySize = config.getClientBodySize();
  this->root = config.getRoot();
  this->errorPage = config.getErrorPage();
}

LocationConfig::~LocationConfig() {}

LocationConfig::LocationConfig(const LocationConfig& obj): 
  CommonConfig(obj),
  path(obj.getPath()),
  alias(obj.getAlias()),
  limitExcept(obj.getLimitExcept()),
  returnRes(obj.getReturnRes()), 
  autoIndex(obj.isAutoIndex()),
  locations(obj.getLocationConfig()) {}

LocationConfig& LocationConfig::operator=(const LocationConfig& obj) {
  if (this != &obj) {
    this->clientBodySize = obj.getClientBodySize();
    this->root = obj.getRoot();
    this->errorPage = obj.getErrorPage();
    this->index = obj.getIndex();

    this->path = obj.getPath();
    this->alias = obj.getAlias();
    this->limitExcept = obj.getLimitExcept();
    this->returnRes = obj.getReturnRes();
    this->autoIndex = obj.isAutoIndex();
    this->locations = obj.getLocationConfig();
  }
  return *this;
}

// getter

std::string LocationConfig::getPath() const {
  return this->path;
}

std::string LocationConfig::getAlias() const {
  return this->alias;
}

std::string LocationConfig::getLimitExcept() const {
  return this->limitExcept;
}

std::map<int, std::string> LocationConfig::getReturnRes() const {
  return this->returnRes;
}

bool LocationConfig::isAutoIndex() const {
  return this->autoIndex;
}

std::vector<LocationConfig> LocationConfig::getLocationConfig() const {
  return this->locations;
}

// setter

void LocationConfig::setPath(std::string path) {
  this->path = path;
}

void LocationConfig::setAlias(std::string alias) {
  this->alias = alias;
}

void LocationConfig::setLimitExcept(std::string limitExcept) {
  this->limitExcept = limitExcept;
}

void LocationConfig::addReturnRes(std::pair<int, std::string> returnRes) {
  this->returnRes.insert(returnRes);
}

void LocationConfig::setAutoIndex(bool autoIndex) {
  this->autoIndex = autoIndex;
}

void LocationConfig::addLocationConfig(LocationConfig location) {
  this->locations.push_back(location);
}
