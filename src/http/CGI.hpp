#ifndef CGI_HPP
# define CGI_HPP

# include <string>
# include <map>
# include <cstring>
# include <fcntl.h>
# include <sys/wait.h>

# include "./HttpRequest.hpp"
# include "../etc/Util.hpp"

static const std::string CGI_VERSION = "CGI/1.1";
static const std::string SOFTWARE_NAME = "NGINX MINUS";

class CGI {
  public:
    CGI();
    ~CGI();
    CGI(const CGI& obj);
    CGI& operator=(const CGI& obj);

    void              initCGI(const HttpRequest& req, const bool sessionAvailable);
    int               writeCGI();
    int               readCGI();

    int               getReadFD() const;
    int               getWriteFD() const;
    int               getPid() const;
    std::string       getCgiResult() const;


  private:
    static const int  READ_BUF_SIZE = 1024 * 5;
    static const int  READ = 0;
    static const int  WRITE = 1;

    pid_t             pid;
    int               read_fd;
    int               write_fd;
    std::string       cgi_result;

    size_t            body_offset;
    const std::string getBody(void) const;
    void              addBodyOffset(size_t s);

    std::string       scriptPath;
    std::string       cgiPath;
    std::string       pathInfo;
    std::string       body;
    bool              sessionAvailable;

    const std::map<std::string, std::string>  getEnvMap(const HttpRequest& req) const;
    char**                                    getArgv() const;
    char**                                    envMapToEnv(const std::map<std::string, std::string>& envMap) const;

    void                                      changeWorkingDirectory(void);

    const std::string                         getScriptPath(void) const;
    const std::string                         getCgiPath(void) const;
    const std::string                         getCurrentPath(void) const;
    const std::string                         getPathInfo(void) const;
    const std::string                         getSessionAvailable(void) const;
    const std::string                         convertHeaderKey(const std::string& key) const;

    void                                      prepareCGI(const HttpRequest& req, const bool sessionAvailable);

};

namespace cgi_env {
  static const std::string CONTENT_LENGTH           = "CONTENT_LENGTH";
  static const std::string CONTENT_TYPE             = "CONTENT_TYPE";
  static const std::string GATEWAY_INTERFACE        = "GATEWAY_INTERFACE";
  static const std::string HTTP_ACCEPT              = "HTTP_ACCEPT";
  static const std::string HTTP_ACCEPT_CHARSET      = "HTTP_ACCEPT_CHARSET";
  static const std::string HTTP_ACCEPT_ENCODING     = "HTTP_ACCEPT_ENCODING";
  static const std::string HTTP_ACCEPT_LANGUAGE     = "HTTP_ACCEPT_LANGUAGE";
  static const std::string HTTP_HOST                = "HTTP_HOST";
  static const std::string HTTP_USER_AGENT          = "HTTP_USER_AGENT";
  static const std::string PATH_INFO                = "PATH_INFO";
  static const std::string PATH_TRANSLATED          = "PATH_TRANSLATED";
  static const std::string QUERY_STRING             = "QUERY_STRING";
  static const std::string REQUEST_METHOD           = "REQUEST_METHOD";
  static const std::string SCRIPT_NAME              = "SCRIPT_NAME";
  static const std::string SERVER_NAME              = "SERVER_NAME";
  static const std::string SERVER_PORT              = "SERVER_PORT";
  static const std::string SERVER_PROTOCOL          = "SERVER_PROTOCOL";
  static const std::string SERVER_SOFTWARE          = "SERVER_NAME";
  static const std::string HTTP_COOKIE              = "HTTP_COOKIE";
  static const std::string SESSION_AVAILABLE        = "SESSION_AVAILABLE";
  static const std::string REQUEST_URI              = "REQUEST_URI";
}

#endif
