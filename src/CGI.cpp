#include "CGI.hpp"
#include "Util.hpp"
#include "http/HttpStatus.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

CGI::CGI(HttpRequest& req) {
  this->argv = this->getArgv(req);
  this->env = this->envMapToEnv(this->getEnvMap(req));
}

/*
 * -------------------------- Destructor ---------------------------
 */

CGI::~CGI(void) {

}

/*
 * -------------------------- Operator -----------------------------
 */

/*
 * -------------------------- Getter -------------------------------
 */

/*
 * -------------------------- Setter -------------------------------
 */

/*
 * ----------------------- Member Function -------------------------
 */

const std::map<std::string, std::string> CGI::getEnvMap(HttpRequest& req) const {
  std::map<std::string, std::string> ret;

  if (!req.getBody().empty()) {
    ret.insert(std::pair<std::string, std::string>(cgi_env::CONTENT_LENGTH, util::itoa(req.getBody().length())));
    ret.insert(std::pair<std::string, std::string>(cgi_env::CONTENT_TYPE, req.getContentType()));
  }
  ret.insert(std::pair<std::string, std::string>(cgi_env::GATEWAY_INTERFACE, CGI_VERSION));
  // config, uri 파싱 필요
  ret.insert(std::pair<std::string, std::string>(cgi_env::PATH_INFO, ""));
  // config, uri 파싱 필요
  ret.insert(std::pair<std::string, std::string>(cgi_env::PATH_TRANSLATED, ""));
  ret.insert(std::pair<std::string, std::string>(cgi_env::QUERY_STRING, this->getQueryString(req.getPath())));
  ret.insert(std::pair<std::string, std::string>(cgi_env::REQUEST_METHOD, req.getMethod()));
  // config, uri 파싱 필요
  ret.insert(std::pair<std::string, std::string>(cgi_env::SCRIPT_NAME, ""));
  // req에 config 추가되면 getHost로 가져와
  ret.insert(std::pair<std::string, std::string>(cgi_env::SERVER_NAME, ""));
  // getPort로 가져와
  ret.insert(std::pair<std::string, std::string>(cgi_env::SERVER_PORT, ""));
  ret.insert(std::pair<std::string, std::string>(cgi_env::SERVER_PROTOCOL, req.getVersion()));
  ret.insert(std::pair<std::string, std::string>(cgi_env::SERVER_SOFTWARE, SOFTWARE_NAME));

  return ret;
}

char** CGI::getArgv(HttpRequest& req) const {
  // config, uri 파싱 필요
}

char** CGI::envMapToEnv(const std::map<std::string, std::string>& envMap) const {
  char** ret;
  
  ret = (char**)malloc(sizeof(char*) * envMap.size() + 1);
  if (ret == NULL) throw INTERNAL_SERVER_ERROR;

  int i = 0;
  for (std::map<std::string, std::string>::const_iterator it = envMap.begin(); it != envMap.end(); ++it) {
    if (!(ret[i] = strdup((it->first + "=" + it->second).c_str()))) throw INTERNAL_SERVER_ERROR;
    i++;
  }
  ret[i] = NULL;
  
  return ret;
}

std::string CGI::execute(void) {
  std::string ret;
  int pid;
  int fd[2];
  int status;
  
  if (access(this->cgiPath.c_str(), X_OK) == 0) throw INTERNAL_SERVER_ERROR;
  if (access(this->scriptPath.c_str(), X_OK) == 0) throw INTERNAL_SERVER_ERROR;
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
    if (execve(this->cgiPath.c_str(), this->argv, this->env) < 0)
      throw INTERNAL_SERVER_ERROR;
    exit(0);
  }

  close(fd[WRITE]);
  waitpid(pid, &status, 0);
  ret = util::readFd(fd[READ]);

  return ret;
}

const std::string CGI::getQueryString(const std::string& path) const {
  std::string ret = "";

  size_t pos = path.find("?");
  if (pos != std::string::npos)
    ret += path.substr(pos);

  return ret;
}

/*
 * ---------------------- Non-Member Function ----------------------
 */
