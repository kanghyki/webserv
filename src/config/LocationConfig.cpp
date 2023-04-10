#include "LocationConfig.hpp"

LocationConfig::LocationConfig() {}
LocationConfig::~LocationConfig() {}
LocationConfig::LocationConfig(const LocationConfig &obj) {}
LocationConfig &LocationConfig::operator=(const LocationConfig &obj) {
  if (this != &obj) {
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
