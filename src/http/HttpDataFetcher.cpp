#include "HttpDataFetcher.hpp"

HttpDataFecther::HttpDataFecther(const HttpRequest& req): req(req) {}

HttpDataFecther::~HttpDataFecther() {}

std::string HttpDataFecther::fetch() const throw(HttpStatus) {
  std::string data;

  std::cout << "relative path: " << this->req.getRelativePath() << std::endl;

  if (this->req.getLocationConfig().isAutoIndex()) {
    std::cout << "autoIndex : true" << std::endl;
    DIR* dir = opendir(this->req.getRelativePath().c_str());
    if (dir) {
      std::cout << "@is dir" << std::endl;
      closedir(dir);
      data = autoindex();
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

std::string HttpDataFecther::autoindex() const throw(HttpStatus) {
  std::string     ret;
  DIR*            dir;
  struct dirent*  ent;

  if ((dir = opendir(req.getRelativePath().c_str())) == NULL) {
    if (errno == ENOTDIR)
      throw (FORBIDDEN);
    if (errno == ENOENT)
      throw (NOT_FOUND);
    else
      throw (INTERNAL_SERVER_ERROR);
  }
  ret = "<!DOCTYPE html>\
    <html>\
    <head>\
    <title>Index of " + req.getPath() + "</title>\
    </head>\
    <style>\
    table { width: 300px; }\
    th { height: 17px; }\
    </style>\
    <body>\
    <h1>Index of " + req.getPath() + "</h1>\
    <table>";

  while ((ent = readdir(dir)) != NULL) {
    std::string name(ent->d_name);
    if (name == ".")
      continue;
    ret += "<tr><td>";
    if (ent->d_type == DT_DIR) ret += "<a href=" + name + "/>" + name + "/<a></td><td align=\"right\">directory";
    else if (ent->d_type == DT_REG) ret += "<a href=" + name + ">" + name + "<a></td><td align=\"right\">file";
    ret += "</td></tr>\n";
  }
  ret += "</table>\
          </body>\
          </html>";

  closedir(dir);

  return ret;
}
