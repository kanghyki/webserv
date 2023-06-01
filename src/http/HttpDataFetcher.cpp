#include "HttpDataFetcher.hpp"

HttpDataFecther::HttpDataFecther(const HttpRequest& req): req(req) {}

HttpDataFecther::~HttpDataFecther() {}

std::string HttpDataFecther::fetch() const {
  std::string _data;
  struct stat _stat;

  if (stat(this->req.getTargetPath().c_str(), &_stat) == -1)
    throw NOT_FOUND;

  if (S_ISDIR(_stat.st_mode)) {
    if (this->req.getLocationConfig().isAutoindex())
      _data = autoindex();
    else if (this->req.getIndexTargetPath() != "")
      _data = readFile(this->req.getIndexTargetPath());
    else
      throw NOT_FOUND;
  }
  else if (S_ISREG(_stat.st_mode))
    _data = readFile(this->req.getTargetPath());
  else
    throw FORBIDDEN;

  return _data;
}

std::string HttpDataFecther::readFile(const std::string& path) {
  std::string ret;

  try {
    ret = util::readFile(path);
  } catch (util::IOException& e) {
    throw NOT_FOUND;
  }

  return ret;
}

std::string HttpDataFecther::autoindex() const {
  std::string     ret;
  DIR*            dir;
  struct dirent*  ent;

  if ((dir = opendir(req.getTargetPath().c_str())) == NULL) {
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
    <style>\
    table { width: 300px; }\
    th { height: 17px; }\
    </style>\
    <title>Index of " + req.getPath() + "</title>\
    </head>\
    <body>\
    <h1>Index of " + req.getPath() + "</h1>\
    <table>";

  while ((ent = readdir(dir)) != NULL) {
    std::string name(ent->d_name);
    if (name == ".")
      continue;
    ret += "<tr><td>";
    if (ent->d_type == DT_DIR) ret += "<a href=" + name + "/>" + name + "/</a></td><td align=\"right\">directory";
    else if (ent->d_type == DT_REG) ret += "<a href=" + name + ">" + name + "</a></td><td align=\"right\">file";
    ret += "</td></tr>\n";
  }
  ret += "</table>\
          </body>\
          </html>";

  closedir(dir);

  return ret;
}
