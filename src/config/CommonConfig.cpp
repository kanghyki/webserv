#include "CommonConfig.hpp"

CommonConfig::CommonConfig() {}
CommonConfig::~CommonConfig() {}
CommonConfig::CommonConfig(const CommonConfig &obj) {}
CommonConfig &CommonConfig::operator=(const CommonConfig &obj) {
  if (this != &obj) {
  }
  return *this;
}

int CommonConfig::getClientBodySize() const {
  return this->clientBodySize;
}

void CommonConfig::setClientBodySize(int clientBodySize) {
  this->clientBodySize = clientBodySize;
}

std::string CommonConfig::getRoot() const {
  return this->root;
}

void CommonConfig::setRoot(std::string root) {
  this->root = root;
}

std::map<int, std::string> CommonConfig::getErrorPage() const {
  return this->errorPage;
}

void CommonConfig::addErrorPage(std::pair<int, std::string> errorPage) {
  this->errorPage.insert(errorPage);
}

std::vector<std::string> CommonConfig::getIndex() const {
  return this->index;
}

void CommonConfig::addIndex(std::string index) {
  this->index.push_back(index);
}
