#include "LocationConfig.hpp"

LocationConfig::LocationConfig(): path(""), alias(""), limitExcept(),
returnRes(), autoIndex(false), locations(0) {}

LocationConfig::~LocationConfig() {}

LocationConfig::LocationConfig(const LocationConfig &obj): 
  CommonConfig(obj),
  path(obj.getPath()),
  alias(obj.getAlias()),
  limitExcept(obj.getLimitExcept()),
  returnRes(obj.getReturnRes()), 
  autoIndex(obj.isAutoIndex()),
  locations(obj.getLocationConfig()) {}

LocationConfig &LocationConfig::operator=(const LocationConfig &obj) {
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

std::string LocationConfig::getPath() const {
  return this->path;
}

void LocationConfig::setPath(std::string path) {
  this->path = path;
}

std::string LocationConfig::getAlias() const {
  return this->alias;
}

void LocationConfig::setAlias(std::string alias) {
  this->alias = alias;
}

std::string LocationConfig::getLimitExcept() const {
  return this->limitExcept;
}

void LocationConfig::setLimitExcept(std::string limitExcept) {
  this->limitExcept = limitExcept;
}

std::map<int, std::string> LocationConfig::getReturnRes() const {
  return this->returnRes;
}

void LocationConfig::addReturnRes(std::pair<int, std::string> returnRes) {
  this->returnRes.insert(returnRes);
}

bool LocationConfig::isAutoIndex() const {
  return this->autoIndex;
}

void LocationConfig::setAutoIndex(bool autoIndex) {
  this->autoIndex = autoIndex;
}

std::vector<LocationConfig> LocationConfig::getLocationConfig() const {
  return this->locations;
}

void LocationConfig::addLocationConfig(LocationConfig location) {
  this->locations.push_back(location);
}
