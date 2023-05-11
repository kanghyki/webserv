#include "./CGI.hpp"

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

  ret.insert(std::pair<std::string, std::string>(cgi_env::PATH_INFO, req.getPath()));
  ret.insert(std::pair<std::string, std::string>(cgi_env::REQUEST_URI, req.getPath()));

  ret.insert(std::pair<std::string, std::string>(cgi_env::PATH_TRANSLATED, getCurrentPath() + req.getSubstitutedPath()));
  ret.insert(std::pair<std::string, std::string>(cgi_env::QUERY_STRING, req.getQueryString()));
  ret.insert(std::pair<std::string, std::string>(cgi_env::REQUEST_METHOD, req.getMethod())); 
  ret.insert(std::pair<std::string, std::string>(cgi_env::SCRIPT_NAME, req.getPath()));
  ret.insert(std::pair<std::string, std::string>(cgi_env::SERVER_NAME, req.getServerConfig().getHost()));
  ret.insert(std::pair<std::string, std::string>(cgi_env::SERVER_PORT, util::itoa(req.getServerConfig().getPort())));
  ret.insert(std::pair<std::string, std::string>(cgi_env::SERVER_PROTOCOL, req.getVersion()));
  ret.insert(std::pair<std::string, std::string>(cgi_env::SERVER_SOFTWARE, SOFTWARE_NAME));
  ret.insert(std::pair<std::string, std::string>(cgi_env::HTTP_COOKIE, req.getHeader().get(cgi_env::HTTP_COOKIE)));
  ret.insert(std::pair<std::string, std::string>(cgi_env::SESSION_AVAILABLE, getSessionAvailable()));

  std::map<std::string, std::string> custom = req.getHeader().getCustomeHeader();
  for (std::map<std::string, std::string>::iterator it = custom.begin(); it != custom.end(); ++it) {
    std::string key = convertHeaderKey(it->first);
    ret.insert(std::pair<std::string, std::string>("HTTP_" + key, it->second));
  }
//
//  for (std::map<std::string, std::string>::iterator it = ret.begin(); it != ret.end(); ++it) {
//    std::cout << it->first << " : " << it->second << std::endl;
//  }

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
  
  ret = (char**)malloc(sizeof(char*) * (envMap.size() + 1));
  if (ret == NULL) throw INTERNAL_SERVER_ERROR;

  int i = 0;
  for (std::map<std::string, std::string>::const_iterator it = envMap.begin(); it != envMap.end(); ++it) {
    if (!(ret[i] = strdup((it->first + "=" + it->second).c_str()))) throw INTERNAL_SERVER_ERROR;
    i++;
  }
  ret[i] = NULL;
  
  return ret;
}

void CGI::execute(fd_set& reads, fd_set& writes) {
  std::string ret;
  pid_t       pid = -1;
  int         fd1[2];
  int         fd2[2];


  if (access(this->cgiPath.c_str(), X_OK) == -1) throw INTERNAL_SERVER_ERROR;


  try {
    util::ftPipe(fd1);
    util::ftPipe(fd2);
    pid = util::ftFork();
  } catch (util::SystemFunctionException& e) {
    throw INTERNAL_SERVER_ERROR;
  }

  if (pid == 0) {
    try {
      close(fd1[WRITE]);
      close(fd2[READ]);
      util::ftDup2(fd1[READ], STDIN_FILENO);
      util::ftDup2(fd2[WRITE], STDOUT_FILENO);
      close(fd1[READ]);
      close(fd2[WRITE]);
      changeWorkingDirectory();
      util::ftExecve(this->cgiPath, this->argv, this->env);
    } catch (util::SystemFunctionException& e) {
      exit(-1);
    }
  }
  this->childPid = pid;
  this->readFd = fd2[READ];
  this->writeFd = fd1[WRITE];

  close(fd1[READ]);
  close(fd2[WRITE]);
  fcntl(fd1[WRITE], F_SETFL, O_NONBLOCK);
  FD_SET(fd1[WRITE], &writes);
  if (write(fd1[WRITE], getBody().c_str(), getBody().length()) == -1)
    throw INTERNAL_SERVER_ERROR;

//  waitpid(pid, &status, 0);
//  if (status == -1)
//    throw INTERNAL_SERVER_ERROR;
//
//  if ((ret = util::readFd(fd_out)).empty())
//    throw INTERNAL_SERVER_ERROR;
}

void CGI::changeWorkingDirectory(void) {
  std::string target = getScriptPath().substr(0, getScriptPath().rfind("/"));

  if (chdir(target.c_str()) == -1) throw util::SystemFunctionException();
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


const std::string CGI::convertHeaderKey(const std::string& key) const {
  std::string ret = util::toUpperStr(key);

  for (size_t i = 0; i < ret.length(); ++i) {
    if(ret[i] == '-')
      ret[i] = '_';
  }

  return ret;
}

pid_t CGI::getChildPid() const{
  return this->childPid;
}

int   CGI::getReadFd() const {
  return this->readFd;
}

int   CGI::getWriteFd () const {
  return this->writeFd;
}

int   CGI::getStatus() const {
  return this->status;
}

/*
 * ---------------------- Non-Member Function ----------------------
 */
