#include "HttpDataFetcher.hpp"
#include "HttpStatus.hpp"

HttpDataFecther::HttpDataFecther(HttpRequest request, ServerConfig config): request(request), config(config) {}

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

std::string HttpDataFecther::fetch() const {
  std::string data;

  std::cout << "Root: " << this->config.getRoot() << std::endl;
  std::cout << "Request path: " << this->request.getPath() << std::endl;

  if (is_regular_file(("." + this->request.getPath()).c_str()))
    data = readFile(this->request.getPath());
  else 
    data = readDirectory();

  return data;
}


std::string HttpDataFecther::readDirectory() const {
  std::string dirpath = "." + this->request.getPath();
  std::cout << "dirpaht" <<  dirpath << std::endl;
  DIR* directory = opendir(dirpath.c_str());
  if (directory == NULL) {
    throw NOT_FOUND;
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
  datastr += "</table>\
</body>\
</html>";
  closedir (directory);
  return datastr;
}

std::string HttpDataFecther::excuteCGI(const std::string& path) const {
  std::string ret;
  int pid;
  int fd[2];
  int status;
  
  if (access(path.c_str(), X_OK) == 0) throw NOT_FOUND;
  try {
    util::ftPipe(fd);
    pid = util::ftFork();
  } catch (util::SystemFunctionException& e) {
    throw INTERNAL_SERVER_ERROR;
  }
  
  if (pid == 0) {
    close(fd[READ]);
    dup2(fd[WRITE], STDOUT_FILENO);
    close(fd[WRITE]);
    char* const p = (char* const)path.c_str();
    char * const args[] = {"python3", p, nullptr};
    if (execve("/usr/bin/python3", args, 0) < 0)
      throw INTERNAL_SERVER_ERROR;
    exit(0);
  }
  close(fd[WRITE]);
  waitpid(pid, &status, 0);
  ret = util::readFd(fd[READ]);

  std::cout << "ret : " << ret << std::endl;

  return ret;
}

std::string HttpDataFecther::readFile(const std::string& path) {
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
