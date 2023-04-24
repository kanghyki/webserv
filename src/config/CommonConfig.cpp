#include "./CommonConfig.hpp"

const int         CommonConfig::DEFAULT_CLIENT_BODY_SIZE = 8192;
const std::string CommonConfig::DEFAULT_ROOT = "/html";
const std::string CommonConfig::DEFAULT_INDEX = "index.html";

CommonConfig::CommonConfig():
  clientBodySize(DEFAULT_CLIENT_BODY_SIZE),
  root(DEFAULT_ROOT),
  errorPagePath(),
  errorPageStatus(),
  index() {}

CommonConfig::CommonConfig(const CommonConfig& obj):
  clientBodySize(obj.getClientBodySize()),
  root(obj.getRoot()),
  errorPagePath(obj.getErrorPagePath()),
  errorPageStatus(obj.getErrorPageStatus()),
  index(obj.getIndex()) {}

CommonConfig::~CommonConfig() {}

CommonConfig &CommonConfig::operator=(const CommonConfig& obj) {
  if (this != &obj) {
    this->clientBodySize = obj.getClientBodySize();
    this->root = obj.getRoot();
    this->errorPagePath = obj.getErrorPagePath();
    this->errorPageStatus = obj.getErrorPageStatus();
    this->index = obj.getIndex();
  }
  return *this;
}

// getter

int CommonConfig::getClientBodySize() const {
  return this->clientBodySize;
}

std::string CommonConfig::getRoot() const {
  return this->root;
}

std::string CommonConfig::getErrorPagePath() const {
  return this->errorPagePath;
}

std::vector<int> CommonConfig::getErrorPageStatus() const {
  return this->errorPageStatus;
}

std::vector<std::string> CommonConfig::getIndex() const {
  if (this->index.size() == 0) {
    std::vector<std::string> ret;
    ret.push_back(DEFAULT_INDEX);
    return ret;
  }
  return this->index;
}

// setter

void CommonConfig::setClientBodySize(int clientBodySize) {
  this->clientBodySize = clientBodySize;
}

void CommonConfig::setRoot(std::string root) {
  this->root = root;
}

void CommonConfig::setErrorPagePath(std::string path) {
  this->errorPagePath = path;
}

void CommonConfig::addErrorPageStatus(int n) {
  this->errorPageStatus.push_back(n);
}

void CommonConfig::addIndex(std::string index) {
  this->index.push_back(index);
}
