#include "CommonConfig.hpp"

const int         CommonConfig::DEFAULT_CLIENT_BODY_SIZE = 8192;
const std::string CommonConfig::DEFAULT_ROOT = "/html";
const std::string CommonConfig::DEFAULT_INDEX = "index.html";

CommonConfig::CommonConfig(): clientBodySize(DEFAULT_CLIENT_BODY_SIZE), root(DEFAULT_ROOT), errorPage(), index() {
  addIndex(DEFAULT_INDEX);
}

CommonConfig::CommonConfig(const CommonConfig& obj):
  clientBodySize(obj.getClientBodySize()),
  root(obj.getRoot()),
  errorPage(obj.getErrorPage()),
  index(obj.getIndex()) {}

CommonConfig::~CommonConfig() {}

CommonConfig &CommonConfig::operator=(const CommonConfig& obj) {
  if (this != &obj) {
    this->clientBodySize = obj.getClientBodySize();
    this->root = obj.getRoot();
    this->errorPage = obj.getErrorPage();
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

std::map<int, std::string> CommonConfig::getErrorPage() const {
  return this->errorPage;
}

std::vector<std::string> CommonConfig::getIndex() const {
  return this->index;
}

// setter

void CommonConfig::setClientBodySize(int clientBodySize) {
  this->clientBodySize = clientBodySize;
}

void CommonConfig::setRoot(std::string root) {
  this->root = root;
}

void CommonConfig::addErrorPage(std::pair<int, std::string> errorPage) {
  this->errorPage.insert(errorPage);
}

void CommonConfig::addIndex(std::string index) {
  this->index.push_back(index);
}
