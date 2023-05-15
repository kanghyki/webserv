#include "./CGI.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

CGI::CGI():
  body_offset(0)
//  status(WRITING),
{}

CGI::CGI(const CGI& obj):
  pid(obj.pid),
  read_fd(obj.read_fd),
  write_fd(obj.write_fd),
  cgi_result(obj.cgi_result),
  body_offset(obj.body_offset),
  scriptPath(obj.scriptPath),
  cgiPath(obj.cgiPath),
  pathInfo(obj.pathInfo),
  body(obj.body),
  sessionAvailable(obj.sessionAvailable) {
}

CGI& CGI::operator=(const CGI& obj) {
  if (this != &obj) {
    pid = obj.pid;
    read_fd = obj.read_fd;
    write_fd = obj.write_fd;
    cgi_result = obj.cgi_result;
    body_offset = obj.body_offset;
    scriptPath = obj.scriptPath;
    cgiPath = obj.cgiPath;
    pathInfo = obj.pathInfo;
    body = obj.body;
    sessionAvailable = obj.sessionAvailable;
  }

  return *this;
}

void CGI::prepareCGI(const HttpRequest& req, const bool sessionAvailable) {
  this->scriptPath = req.getScriptPath();
  this->cgiPath = req.getCGIPath();
  this->pathInfo = req.getPathInfo();
  this->sessionAvailable = sessionAvailable;
  if (!req.getBody().empty())
    this->body = req.getBody();
}

int CGI::getReadFD() const {
  return this->read_fd;
}

int CGI::getWriteFD() const {
  return this->write_fd;
}

int CGI::getPid() const {
  return this->pid;
}

std::string CGI::getCgiResult() const {
  return this->cgi_result;
}

/*
 * -------------------------- Destructor ---------------------------
 */

CGI::~CGI() {}

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

void CGI::addBodyOffset(size_t s) {
  this->body_offset += s;
}


/*
 * -------------------------- Setter -------------------------------
 */

/*
 * ----------------------- Member Function -------------------------
 */

void CGI::initCGI(const HttpRequest& req, const bool sessionAvailable) {
  int     pipe1[2];
  int     pipe2[2];

  prepareCGI(req, sessionAvailable);

  if (pipe(pipe1) == -1) {
    throw INTERNAL_SERVER_ERROR;
  }
  if (pipe(pipe2) == -1) {
    close(pipe1[0]);
    close(pipe1[1]);
    throw INTERNAL_SERVER_ERROR;
  }
  if ((this->pid = fork()) == -1) {
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);
    throw INTERNAL_SERVER_ERROR;
  }

  if (this->pid == 0) {
    char**  argv;
    char**  env;

    argv = this->getArgv();
    env = this->envMapToEnv(this->getEnvMap(req));

    close(pipe1[READ]);
    dup2(pipe1[WRITE], STDOUT_FILENO);
    close(pipe1[WRITE]);

    close(pipe2[WRITE]);
    dup2(pipe2[READ], STDIN_FILENO);
    close(pipe2[READ]);
    changeWorkingDirectory();
    execve(this->cgiPath.c_str(), argv, env);
    exit(EXIT_FAILURE);
  }

  close(pipe1[WRITE]);
  close(pipe2[READ]);

  this->read_fd = pipe1[READ];
  this->write_fd = pipe2[WRITE];

  fcntl(this->write_fd, F_SETFL, O_NONBLOCK);
  fcntl(this->read_fd, F_SETFL, O_NONBLOCK);
}

int CGI::writeCGI() {
  std::string body;
  int         write_size;

  logger::debug << "body size: " << getBody().length() << logger::endl;
  body = getBody().substr(this->body_offset);
  write_size = write(this->write_fd, body.c_str(), body.length());
  if (write_size > 0)
    this->body_offset += write_size;

  return write_size;
}

int CGI::readCGI() {
  char  buf[READ_BUF_SIZE + 1];
  int   read_size;

  read_size = read(this->read_fd, buf, READ_BUF_SIZE);
  buf[read_size] = 0;
  if (read_size > 0)
    this->cgi_result += std::string(buf, read_size);

  return read_size;
}

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

  return ret;
}

char** CGI::getArgv() const {
  char** ret;

  ret = (char**)malloc(sizeof(char*) * 3);
  // FIXME: throw
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

void CGI::changeWorkingDirectory(void) {
  std::string target = getScriptPath().substr(0, getScriptPath().rfind("/"));

  // FIXME: throw
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

/*
 * ---------------------- Non-Member Function ----------------------
 */
