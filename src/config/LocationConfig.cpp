#include "./LocationConfig.hpp"
#include "./ServerConfig.hpp"

const bool LocationConfig::DEFAULT_AUTOINDEX = false;
const bool LocationConfig::DEFAULT_CGI = false;

LocationConfig::LocationConfig(): autoIndex(DEFAULT_AUTOINDEX), cgi(DEFAULT_CGI) {}

LocationConfig::LocationConfig(const ServerConfig& config):
  CommonConfig(config),
  autoIndex(DEFAULT_AUTOINDEX),
  cgi(DEFAULT_CGI) {}

LocationConfig::LocationConfig(const LocationConfig& obj):
  CommonConfig(obj),
  path(obj.getPath()),
  alias(obj.getAlias()),
  cgiPath(obj.getCGIPath()),
  cgiScriptPath(obj.getCGIScriptPath()),
  limitExcept(obj.getLimitExcept()),
  returnRes(obj.getReturnRes()), 
  autoIndex(obj.isAutoIndex()),
  cgi(obj.isCgi()),
  executable(obj.isExecutable()),
  locations(obj.getLocationConfig()) {}

LocationConfig::~LocationConfig() {}

LocationConfig& LocationConfig::operator=(const LocationConfig& obj) {
  if (this != &obj) {
    this->clientBodySize = obj.getClientBodySize();
    this->root = obj.getRoot();
    this->errorPage = obj.getErrorPage();
    this->index = obj.getIndex();

    this->path = obj.getPath();
    this->alias = obj.getAlias();
    this->cgiPath = obj.getCGIPath();
    this->cgiScriptPath = obj.getCGIScriptPath();
    this->limitExcept = obj.getLimitExcept();
    this->returnRes = obj.getReturnRes();
    this->autoIndex = obj.isAutoIndex();
    this->cgi = obj.isCgi();
    this->executable = obj.isExecutable();
    this->locations = obj.getLocationConfig();
  }

  return *this;
}

// getter

std::string LocationConfig::getPath() const { return this->path; }

std::string LocationConfig::getAlias() const { return this->alias; }

std::string LocationConfig::getCGIPath() const { return this->cgiPath; }

std::string LocationConfig::getCGIScriptPath() const { return this->cgiScriptPath; }

std::string LocationConfig::getLimitExcept() const { return this->limitExcept; }

std::map<int, std::string> LocationConfig::getReturnRes() const { return this->returnRes; }

bool LocationConfig::isAutoIndex() const { return this->autoIndex; }

bool LocationConfig::isCgi() const { return this->cgi; }

bool LocationConfig::isExecutable() const { return this->executable; }

const std::vector<LocationConfig>& LocationConfig::getLocationConfig() const { return this->locations; }

// setter

void LocationConfig::setPath(std::string path) { this->path = path; }

void LocationConfig::setAlias(std::string alias) { this->alias = alias; }

void LocationConfig::setLimitExcept(std::string limitExcept) { this->limitExcept = limitExcept; }

void LocationConfig::setCGIPath(std::string p) { this->cgiPath = p; }

void LocationConfig::setCGIScriptPath(std::string p) { this->cgiScriptPath = p; }

void LocationConfig::addReturnRes(std::pair<int, std::string> returnRes) { this->returnRes.insert(returnRes); }

void LocationConfig::setAutoIndex(bool autoIndex) { this->autoIndex = autoIndex; }

void LocationConfig::addLocationConfig(LocationConfig location) { this->locations.push_back(location); }

void LocationConfig::setCgi(bool cgi) { this->cgi = cgi; }

void LocationConfig::setExecutable(bool executable) { this->executable = executable; }
