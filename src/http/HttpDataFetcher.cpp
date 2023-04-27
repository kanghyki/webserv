#include "HttpDataFetcher.hpp"

HttpDataFecther::HttpDataFecther(const HttpRequest& req): req(req) {}

HttpDataFecther::~HttpDataFecther() {}

#include <dirent.h>
#include "./DirectoryList.hpp"
std::string HttpDataFecther::fetch() const throw(HttpStatus) {
  std::string data;

  std::cout << "Root: " << this->req.getConfig().getRoot() << std::endl;
  std::cout << "Request path: " << this->req.getPath() << std::endl;

  DIR* dir = opendir(("." + this->req.getPath()).c_str());
  if (dir) {
    std::cout << "is dir" << std::endl;
    closedir(dir);
    data = DirectoryList::generate(this->req);
  }
  else {
    std::cout << "is file" << std::endl;
    data = getData();
  }

  return data;
}

std::string HttpDataFecther::excuteCGI(const std::string& path) const {
  return "5";
}

std::string HttpDataFecther::readFile(const std::string& path) throw(HttpStatus) {
  std::string ret;

  try {
    ret = util::readFile("." + path);
  } catch (util::IOException& e) {
    throw NOT_FOUND;
  }

  return ret;
}

const std::string HttpDataFecther::getData(void) const throw(HttpStatus) {
  return readFile(this->req.getPath());
}

const std::string HttpDataFecther::getMimeType(void) const throw(HttpStatus) {
  std::string ret;

  try {
    ret = util::getMimeType("." + this->req.getPath());
  } catch (util::IOException& e) {
    throw NOT_FOUND;
  }

  return ret;
}
