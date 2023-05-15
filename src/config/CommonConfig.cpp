#include "./CommonConfig.hpp"

const int         CommonConfig::DEFAULT_CLIENT_BODY_SIZE = 8192;
const std::string CommonConfig::DEFAULT_ROOT = "/html";
const std::string CommonConfig::DEFAULT_INDEX = "index.html";

CommonConfig::CommonConfig():
  clientMaxBodySize(DEFAULT_CLIENT_BODY_SIZE),
  root(DEFAULT_ROOT),
  index(DEFAULT_INDEX),
  errorPage() {}

CommonConfig::CommonConfig(const CommonConfig& obj):
  clientMaxBodySize(obj.getClientMaxBodySize()),
  root(obj.getRoot()),
  index(obj.getIndex()),
  errorPage(obj.getErrorPage()) {}

CommonConfig::~CommonConfig() {}

CommonConfig &CommonConfig::operator=(const CommonConfig& obj) {
  if (this != &obj) {
    this->clientMaxBodySize = obj.getClientMaxBodySize();
    this->root = obj.getRoot();
    this->index = obj.getIndex();
    this->errorPage = obj.getErrorPage();
  }
  return *this;
}

// getter

int CommonConfig::getClientMaxBodySize() const { return this->clientMaxBodySize; }

std::string CommonConfig::getRoot() const { return this->root; }

std::map<int, std::string> CommonConfig::getErrorPage() const {
  return this->errorPage;
}

std::string CommonConfig::getIndex() const {
  return this->index;
}

std::string CommonConfig::trimChar(std::string s, char c) const {
  size_t f, r;

  f = 0;
  while (f < s.length() && s[f] == c)
    ++f;
  s.erase(0, f);
  r = s.length() - 1;
  while (r > 0 && s[f] == c)
    --r;
  s.erase(r + 1, s.length() - 1 - r);

  return s;
}

std::string CommonConfig::combinePath(const std::string& s1, const std::string& s2) const {
  std::string ret;
  std::string ts1 = trimChar(s1, '/');
  std::string ts2 = trimChar(s2, '/');

  if (ts1.size() > 0)
    ret += "/" + ts1;
  if (ts2.size() > 0)
    ret += "/" + ts2;

  if (ret == "")
    ret = "/";

  return ret;
}

std::string CommonConfig::getErrorPageTargetPath(int status) const {
  std::string errorPagePath = getErrorPage()[status];
  std::string ret;

  if (errorPagePath == "")
    return "";

  ret = combinePath(this->root, errorPagePath);

  return "." + ret;
}


// setter

void CommonConfig::setClientMaxBodySize(int n) { this->clientMaxBodySize = n; }

void CommonConfig::setRoot(std::string root) { this->root = root; }

void CommonConfig::addErrorPage(int statusCode, std::string path) {
  this->errorPage.insert(std::make_pair(statusCode, path));
}

void CommonConfig::setIndex(std::string index) { this->index = index; }
