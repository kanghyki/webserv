#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include "./header/HttpRequestHeader.hpp"
# include "./HttpStatus.hpp"
# include "./MimeType.hpp"
# include "../etc/Util.hpp"
# include "../config/Config.hpp"
# include "../etc/Logger.hpp"

# include <cstring>
# include <sys/stat.h>
# include <sstream>
# include <stdexcept>
# include <string>
# include <vector>
# include <map>

namespace request_method {
  const std::string GET    = "GET";
  const std::string POST   = "POST";
  const std::string DELETE = "DELETE";
  const std::string PUT    = "PUT";
  const std::string HEAD   = "HEAD";
}

class HttpRequest {

  public:
    enum recvStatus {
      HEADER_RECEIVE,
      BODY_RECEIVE,
      RECEIVE_DONE,
      RECEIVE_ERROR
    };

    HttpRequest();
    ~HttpRequest();
    HttpRequest& operator=(const HttpRequest& obj);
    HttpRequest(const HttpRequest& obj);

    void                                  parse(const std::string& req, const Config& conf);
    void                                  unchunkBody();

    std::string                           getMethod() const;
    bool                                  isMethod(std::string method) const;
    std::string                           getPath() const;
    std::string                           getSubstitutedPath() const;
    std::string                           getTargetPath() const;
    std::string                           getIndexTargetPath() const;
    std::string                           getQueryString() const;
    std::string                           getVersion() const;
    const HttpRequestHeader&              getHeader() const;
    std::string                           getBody() const;
    const std::string                     getContentType(void) const;
    const LocationConfig&                 getLocationConfig() const;
    const ServerConfig&                   getServerConfig() const;
    bool                                  isCGI() const;
    const std::string                     getScriptPath() const;
    const std::string                     getCGIPath() const;
    const std::string                     getPathInfo() const;

    bool                                  isRecvStatus(recvStatus rs) const;
    int                                   getContentLength() const;
    HttpStatus                            getErrorStatusCode() const;

    void                                  setBody(const std::string& body);

    void                                  setRecvStatus(recvStatus status);
    void                                  setContentLength(int len);
    void                                  setError(HttpStatus status);
    void                                  setConnection(HttpRequestHeader::connection c);

  private:
    static const size_t                   URL_MAX_LENGTH;

    std::string                           method;
    std::string                           path;
    std::string                           queryString;
    std::string                           version;
    std::string                           body;
    HttpRequestHeader                     header;
    ServerConfig                          sc;
    LocationConfig                        lc;

    bool                                  cgi;
    std::string                           scriptPath;
    std::string                           cgiPath;
    std::string                           pathInfo;

    recvStatus                            recv_status;
    int                                   contentLength;
    HttpStatus                            errorStatusCode;

    void                                  parseStatusLine(const std::string &line);
    void                                  setupCGI();

    void                                  setURI(const std::string& URI);
    void                                  setMethod(const std::string& method);
    void                                  setVersion(const std::string& version);

    void                                  validateMethod(const std::string &method);
    void                                  validateVersion(const std::string &path);
    void                                  validateURI(const std::string &version);
    unsigned int                          strToHex(std::string s);
};

#endif
