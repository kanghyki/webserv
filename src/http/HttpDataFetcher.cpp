#include "HttpDataFetcher.hpp"

HttpDataFecther::HttpDataFecther(HttpRequest request, ServerConfig config): request(request), config(config) {}

HttpDataFecther::~HttpDataFecther() {}

std::string HttpDataFecther::fetch() const {
  std::string data;

  std::cout << "Root: " << this->config.getRoot() << std::endl;
  std::cout << "Request path: " << this->request.getPath() << std::endl;

  try {
    data = readFile(this->request.getPath());
  } catch (util::FileOpenException& e) {
    throw NOT_FOUND;
  }

  return data;
}

std::string HttpDataFecther::readDirectory() const {
  std::string dirpath = "." + this->config.getRoot() + this->request.getPath();
  DIR* directory = opendir(dirpath.c_str());
  if (directory == NULL) {
    return "";
  }

  struct dirent* entry;
  std::string datastr = \
                        "<html>\n\
                        <head>\n\
                        <title>Index of " + this->request.getPath()\
                        + "</title>\n\
                        </head>\n\
                        <body>\n\
                        <h1>Index of " + this->request.getPath()\
                        + "</h1>\n\
                        <table>\n\
                        <tr><th align=\"left\">Name</th><th>Type</th><th>Size</th></tr>\
                        \n\
                        ";
  while ((entry = readdir(directory)) != NULL) {
    datastr += "<tr>";
    datastr += "<td><a href=\"";
    if (std::string(entry->d_name) == ".")
      datastr += this->request.getPath();
    else if (this->request.getPath() != "/")
      datastr += this->request.getPath() + "/" + entry->d_name;
    else 
      datastr += this->request.getPath() + entry->d_name;
    datastr += "\">";
    datastr += entry->d_name;
    datastr += "<a></td>\n";


    datastr += "<td align=\"right\">";
    if (entry->d_type == DT_DIR) {
      datastr += "Directory";
    } else if (entry->d_type == DT_REG) {
      datastr += "File";
    }
    datastr += "</td>";

    datastr += "<td align=\"right\">";
    datastr += std::to_string(entry->d_reclen) + "byte";
    datastr += "</td>";

    datastr += "</tr>\n";
  }
  datastr += "</table>";
  closedir (directory);
  return datastr;
}

std::string HttpDataFecther::excuteCGI(const std::string &path) const {
  return "5";
}

std::string HttpDataFecther::readFile(const std::string &path) {
  std::string data = util::readFile("." + path);

  return data;
}
