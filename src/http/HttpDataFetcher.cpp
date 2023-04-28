#include "HttpDataFetcher.hpp"
#include "HttpStatus.hpp"

HttpDataFecther::HttpDataFecther(const HttpRequest& req): req(req) {}

HttpDataFecther::~HttpDataFecther() {}

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

std::string HttpDataFecther::fetch() const throw(HttpStatus) {
  std::string data;

  std::cout << "relative path: " << this->req.getRelativePath() << std::endl;

  if (this->request.getPath().rfind(".py") != std::string::npos)
    data = excuteCGI("." + this->request.getPath());
  else if (is_regular_file(("." + this->request.getPath()).c_str()))
    data = readFile(this->request.getPath());
  else 
    data = readDirectory();

  return data;
}

//TODO: readFile 되살리기

const std::string HttpDataFecther::getData(void) const {
  return readFile(this->request.getPath());
}
