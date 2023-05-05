#include "CGI.hpp"
#include "Util.hpp"
#include "http/HttpStatus.hpp"
#include <utility>

/*
 * -------------------------- Constructor --------------------------
 */

CGI::CGI(const HttpRequest& req, const bool sessionAvailable) : scriptPath(req.getScriptPath()), cgiPath(req.getCGIPath()), pathInfo(req.getPathInfo()), sessionAvailable(sessionAvailable) {
  if (!req.getBody().empty()) this->body = req.getBody();
  this->argv = this->getArgv();
  this->env = this->envMapToEnv(this->getEnvMap(req));
}

/*
 * -------------------------- Destructor ---------------------------
 */

CGI::~CGI(void) {
  util::ftFree(this->argv);
  util::ftFree(this->env);
}

/*
 * -------------------------- Operator -----------------------------
 */

/*
 * -------------------------- Getter -------------------------------
 */

const std::string CGI::getScriptPath(void) const {
  return this->scriptPath;
}

const std::string CGI::getCgiPath(void) const {
  return this->cgiPath;
}

const std::string CGI::getPathInfo(void) const {
  return this->pathInfo;
}

const std::string CGI::getBody(void) const {
  return this->body;
}


/*
 * -------------------------- Setter -------------------------------
 */

/*
 * ----------------------- Member Function -------------------------
 */

const std::map<std::string, std::string> CGI::getEnvMap(const HttpRequest& req) const {
  std::map<std::string, std::string> ret;

  if (!req.getBody().empty()) {
    ret.insert(std::pair<std::string, std::string>(cgi_env::CONTENT_LENGTH, util::itoa(req.getBody().length())));
    ret.insert(std::pair<std::string, std::string>(cgi_env::CONTENT_TYPE, req.getContentType()));
  }
  ret.insert(std::pair<std::string, std::string>(cgi_env::GATEWAY_INTERFACE, CGI_VERSION));
  ret.insert(std::pair<std::string, std::string>(cgi_env::HTTP_ACCEPT, req.getHeader().get(cgi_env::HTTP_ACCEPT)));
  ret.insert(std::pair<std::string, std::string>(cgi_env::HTTP_ACCEPT_CHARSET, req.getHeader().get(cgi_env::HTTP_ACCEPT_CHARSET)));
  ret.insert(std::pair<std::string, std::string>(cgi_env::HTTP_ACCEPT_ENCODING, req.getHeader().get(cgi_env::HTTP_ACCEPT_ENCODING)));
  ret.insert(std::pair<std::string, std::string>(cgi_env::HTTP_ACCEPT_LANGUAGE, req.getHeader().get(cgi_env::HTTP_ACCEPT_LANGUAGE)));
  ret.insert(std::pair<std::string, std::string>(cgi_env::HTTP_HOST, req.getHeader().get(cgi_env::HTTP_HOST)));
  ret.insert(std::pair<std::string, std::string>(cgi_env::HTTP_USER_AGENT, req.getHeader().get(cgi_env::HTTP_USER_AGENT)));
  ret.insert(std::pair<std::string, std::string>(cgi_env::PATH_INFO, getPathInfo()));
  ret.insert(std::pair<std::string, std::string>(cgi_env::PATH_TRANSLATED, getCurrentPath() + req.getPath()));
  ret.insert(std::pair<std::string, std::string>(cgi_env::QUERY_STRING, req.getQueryString()));
  ret.insert(std::pair<std::string, std::string>(cgi_env::REQUEST_METHOD, req.getMethod())); 
  ret.insert(std::pair<std::string, std::string>(cgi_env::SCRIPT_NAME, req.getPath()));
  ret.insert(std::pair<std::string, std::string>(cgi_env::SERVER_NAME, req.getServerConfig().getHost()));
  ret.insert(std::pair<std::string, std::string>(cgi_env::SERVER_PORT, util::itoa(req.getServerConfig().getPort())));
  ret.insert(std::pair<std::string, std::string>(cgi_env::SERVER_PROTOCOL, req.getVersion()));
  ret.insert(std::pair<std::string, std::string>(cgi_env::SERVER_SOFTWARE, SOFTWARE_NAME));
  ret.insert(std::pair<std::string, std::string>(cgi_env::HTTP_COOKIE, req.getHeader().get(cgi_env::HTTP_COOKIE)));
  ret.insert(std::pair<std::string, std::string>(cgi_env::SESSION_AVAILABLE, getSessionAvailable()));

  for (std::map<std::string, std::string>::iterator it = ret.begin(); it != ret.end(); ++it) {
    std::cout << it->first << " : " << it->second << std::endl;
  }

  return ret;
}

char** CGI::getArgv() const {
  char** ret;

  ret = (char**)malloc(sizeof(char*) * 3);
  if (ret == NULL) throw INTERNAL_SERVER_ERROR;

  ret[0] = strdup(getCgiPath().c_str());
  ret[1] = strdup(("./" + getScriptPath().substr(getScriptPath().rfind("/") + 1)).c_str());
  ret[2] = NULL;

  return ret;
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
  int fd1[2];
  int fd2[2];
  int status;

  if (access(this->cgiPath.c_str(), X_OK) == -1) throw INTERNAL_SERVER_ERROR;
  if (access(this->scriptPath.c_str(), X_OK) == -1) throw INTERNAL_SERVER_ERROR;
  try {
    util::ftPipe(fd1);
    util::ftPipe(fd2);
//    fcntl(fd[READ], F_SETFL, O_NONBLOCK);
//    FD_SET(fd[READ], &this->reads);
//    if (fd[READ] > fdMax)
//      fdMax = fd[READ];
    pid = util::ftFork();
  } catch (util::SystemFunctionException& e) {
    throw INTERNAL_SERVER_ERROR;
  }
  
  if (pid == 0) {
    close(fd1[READ]);
    close(fd2[WRITE]);
    dup2(fd1[WRITE], STDOUT_FILENO);
    dup2(fd2[READ], STDIN_FILENO);
    close(fd1[WRITE]);
    close(fd2[READ]);
    changeWorkingDirectory();
    if (execve(this->cgiPath.c_str(), this->argv, this->env) < 0) throw INTERNAL_SERVER_ERROR;
    exit(0);
  }

  close(fd1[WRITE]);
  close(fd2[READ]);
  write(fd2[WRITE], getBody().c_str(), getBody().length());
  close(fd2[WRITE]);
  waitpid(pid, &status, 0);
  ret = util::readFd(fd1[READ]);

  return ret;
}

void CGI::changeWorkingDirectory(void) {
  std::string target = getScriptPath().substr(0, getScriptPath().rfind("/"));

  if (chdir(target.c_str()) == -1) throw INTERNAL_SERVER_ERROR;
}

const std::string CGI::getCurrentPath(void) const {
  char* cur = getcwd(NULL, 0);
  if (!cur) throw INTERNAL_SERVER_ERROR;

  std::string ret = cur;
  free(cur);

  return ret;
}

const std::string CGI::getSessionAvailable(void) const {
  if (this->sessionAvailable)
    return "true";
  return "false";
}

/*
 * ---------------------- Non-Member Function ----------------------
 */
