#include "./LocationConfig.hpp"
#include "./ServerConfig.hpp"

const bool        LocationConfig::DEFAULT_AUTOINDEX = false;
const std::string LocationConfig::DEFAULT_PATH = "/";

LocationConfig::LocationConfig():
  CommonConfig(),
  path(DEFAULT_PATH),
  returnRes(std::make_pair(-1, "")),
  autoindex(DEFAULT_AUTOINDEX) {}

LocationConfig::LocationConfig(const ServerConfig& config):
  CommonConfig(config),
  path(DEFAULT_PATH),
  returnRes(std::make_pair(-1, "")),
  autoindex(DEFAULT_AUTOINDEX) {}

LocationConfig::LocationConfig(const LocationConfig& obj):
  CommonConfig(obj),
  path(obj.getPath()),
  limitExcept(obj.getLimitExcept()),
  returnRes(obj.getReturnRes()), 
  autoindex(obj.isAutoindex()),
  locations(obj.getLocationConfig()) {}

LocationConfig::~LocationConfig() {}

LocationConfig& LocationConfig::operator=(const LocationConfig& obj) {
  if (this != &obj) {
    this->clientMaxBodySize = obj.getClientMaxBodySize();
    this->root = obj.getRoot();
    this->errorPage = obj.getErrorPage();
    this->index = obj.getIndex();

    this->path = obj.getPath();
    this->limitExcept = obj.getLimitExcept();
    this->returnRes = obj.getReturnRes();
    this->autoindex = obj.isAutoindex();
    this->locations = obj.getLocationConfig();
  }

  return *this;
}

// getter

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

std::pair<int, std::string> LocationConfig::getReturnRes() const { return this->returnRes; }

bool LocationConfig::isAutoindex() const { return this->autoindex; }

const std::vector<LocationConfig>& LocationConfig::getLocationConfig() const {
  return this->locations;
}

// setter

void LocationConfig::setPath(std::string path) { this->path = path; }

void LocationConfig::addLimitExcept(std::string method) { this->limitExcept.push_back(method); }

void LocationConfig::setReturnRes(int status, std::string path) {
  this->returnRes = std::make_pair(status, path);
}

void LocationConfig::setAutoindex(bool autoIndex) { this->autoindex = autoIndex; }

void LocationConfig::addLocationConfig(LocationConfig location) { this->locations.push_back(location); }
