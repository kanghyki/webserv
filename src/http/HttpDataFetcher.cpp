#include "HttpDataFetcher.hpp"

HttpDataFecther::HttpDataFecther(HttpRequest request, ServerConfig config): request(request), config(config) {}

HttpDataFecther::~HttpDataFecther() {}

std::string HttpDataFecther::fetch() const {
  std::string data;

  std::cout << "Root: " << this->config.getRoot() << std::endl;
  std::cout << "Request path: " << this->request.getPath() << std::endl;

  data = readFile(this->request.getPath());

  return data;
}

std::string HttpDataFecther::excuteCGI(const std::string &path) const {
  return "5";
}

std::string HttpDataFecther::readFile(const std::string &path) {
  std::string ret;

  try {
    ret = util::readFile("." + path);
  } catch (util::IOException& e) {
    throw NOT_FOUND;
  }

  return ret;
}

const std::string HttpDataFecther::getData(void) const {
  return readFile(this->request.getPath());
}

const std::string HttpDataFecther::getMimeType(void) const {
  std::string ret;

  try {
    ret = util::getMimeType("." + this->request.getPath());
  } catch (util::IOException& e) {
    throw NOT_FOUND;
  }

  return ret;
}
