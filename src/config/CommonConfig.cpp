#include "./CommonConfig.hpp"

const int         CommonConfig::DEFAULT_CLIENT_BODY_SIZE = 8192;
const std::string CommonConfig::DEFAULT_ROOT = "/html";
const std::string CommonConfig::DEFAULT_INDEX = "index.html";

CommonConfig::CommonConfig():
  clientBodySize(DEFAULT_CLIENT_BODY_SIZE),
  root(DEFAULT_ROOT),
  index(DEFAULT_INDEX),
  errorPage() {}

CommonConfig::CommonConfig(const CommonConfig& obj):
  clientBodySize(obj.getClientBodySize()),
  root(obj.getRoot()),
  index(obj.getIndex()),
  errorPage(obj.getErrorPage()) {}

CommonConfig::~CommonConfig() {}

//CommonConfig &CommonConfig::operator=(const CommonConfig& obj) {
//  if (this != &obj) {
//    this->clientBodySize = obj.getClientBodySize();
//    this->root = obj.getRoot();
//    this->index = obj.getIndex();
//    this->errorPage = obj.getErrorPage();
//  }
//  return *this;
//}

// getter

int CommonConfig::getClientBodySize() const { return this->clientBodySize; }

std::string CommonConfig::getRoot() const { return this->root; }

std::map<int, std::string> CommonConfig::getErrorPage() const {
  return this->errorPage;
}

std::string CommonConfig::getIndex() const {
  return this->index;
}

// setter

void CommonConfig::setClientBodySize(int n) { this->clientBodySize = n; }

void CommonConfig::setRoot(std::string root) { this->root = root; }

void CommonConfig::addErrorPage(int statusCode, std::string path) {
  this->errorPage.insert(std::make_pair(statusCode, path));
}

void CommonConfig::setIndex(std::string index) { this->index = index; }
