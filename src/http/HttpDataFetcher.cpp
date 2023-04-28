#include "HttpDataFetcher.hpp"

HttpDataFecther::HttpDataFecther(const HttpRequest& req): req(req) {}

HttpDataFecther::~HttpDataFecther() {}

std::string HttpDataFecther::fetch() const throw(HttpStatus) {
  std::string data;

  std::cout << "relative path: " << this->req.getRelativePath() << std::endl;

  if (this->req.getConfig().isAutoIndex()) {
    std::cout << "autoIndex : true" << std::endl;
    DIR* dir = opendir(this->req.getRelativePath().c_str());
    if (dir) {
      std::cout << "@is dir" << std::endl;
      closedir(dir);
      data = DirectoryList::generate(this->req);
    }
    else {
      std::cout << "@is file" << std::endl;
      data = getData();
    }
  }
  else {
    std::cout << "autoIndex : false" << std::endl;
    // FIXME :: 폴더 읽어도 에러 안남.., 데이터는 0 임
    data = getData();
  }

  return data;
}

std::string HttpDataFecther::readFile(const std::string& path) throw(HttpStatus) {
  std::string ret;

  try {
    ret = util::readFile(path);
  } catch (util::IOException& e) {
    throw NOT_FOUND;
  }

  return ret;
}

const std::string HttpDataFecther::getData(void) const throw(HttpStatus) {
  return readFile(this->req.getRelativePath());
}
