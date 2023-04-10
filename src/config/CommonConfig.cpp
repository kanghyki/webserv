#include "CommonConfig.hpp"

CommonConfig::CommonConfig(): clientBodySize(0), root("/"), errorPage(), index(0) {}

CommonConfig::~CommonConfig() {}

CommonConfig::CommonConfig(const CommonConfig &obj):
  clientBodySize(obj.getClientBodySize()),
  root(obj.getRoot()),
  errorPage(obj.getErrorPage()),
  index(obj.getIndex()) {}

CommonConfig &CommonConfig::operator=(const CommonConfig &obj) {
  if (this != &obj) {
    this->clientBodySize = obj.getClientBodySize();
    this->root = obj.getRoot();
    this->errorPage = obj.getErrorPage();
    this->index = obj.getIndex();
  }
  return *this;
}

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
