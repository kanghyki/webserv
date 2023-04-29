#ifndef CGI_HPP
# define CGI_HPP

# include <string>
# include <map>
# include <cstring>
# include <fcntl.h>

# include "./http/HttpRequest.hpp"
# include "./Util.hpp"

static const std::string CGI_VERSION = "CGI/1.1";
static const std::string SOFTWARE_NAME = "NGINX MINUS";
static const std::string UPLOAD_DIR = "/upload";

class CGI {
  public:
    CGI(const HttpRequest& req, fd_set& reads, int& fdMax);
    ~CGI(void);
    std::string execute(void);

  private:
    char** argv;
    char** env;
    std::string scriptPath;
    std::string cgiPath;
    std::string pathInfo;
    std::string body;
    fd_set& reads;
    int& fdMax;

    const std::map<std::string, std::string> getEnvMap(const HttpRequest& req) const;
    char** getArgv(const HttpRequest& req) const;
    char** envMapToEnv(const std::map<std::string, std::string>& envMap) const;

//    const std::string getPathInfo(const HttpRequest& req) const;
    void changeWorkingDirectory(void);

    const std::string getScriptPath(void) const;
    const std::string getCgiPath(void) const;
    const std::string getCurrentPath(void) const;
    const std::string getPathInfo(void) const;
    const std::string getBody(void) const;
    const std::string getUploadDir(void) const;

    static const int READ = 0;
    static const int WRITE = 1;
};

namespace cgi_env {
  static const std::string CONTENT_LENGTH           = "CONTENT_LENGTH";
  static const std::string CONTENT_TYPE             = "CONTENT_TYPE";
  static const std::string GATEWAY_INTERFACE        = "GATEWAY_INTERFACE";
  static const std::string PATH_INFO                = "PATH_INFO";
  static const std::string PATH_TRANSLATED          = "PATH_TRANSLATED";
  static const std::string QUERY_STRING             = "QUERY_STRING";
  static const std::string REQUEST_METHOD           = "REQUEST_METHOD";
  static const std::string SCRIPT_NAME              = "SCRIPT_NAME";
  static const std::string SERVER_NAME              = "SERVER_NAME";
  static const std::string SERVER_PORT              = "SERVER_PORT";
  static const std::string SERVER_PROTOCOL          = "SERVER_PROTOCOL";
  static const std::string SERVER_SOFTWARE          = "SERVER_NAME";
}

#endif
