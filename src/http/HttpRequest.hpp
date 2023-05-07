#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include "./HttpRequestHeader.hpp"
# include "./HttpHeaderField.hpp"
# include "./HttpStatus.hpp"
# include "./HttpHeaderField.hpp"
# include "../Util.hpp"
# include "../MimeType.hpp"
# include "../config/Config.hpp"
# include "../Logger.hpp"

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
      RECEIVE_DONE
    };

    HttpRequest();
    ~HttpRequest();
    HttpRequest& operator=(const HttpRequest& obj);

    void                                  parse(const std::string& req, const Config& conf) throw (HttpRequest);
    void                                  checkCGI();

    std::string                           getMethod() const;
    std::string                           getPath() const;
    std::string                           getSubstitutedPath() const;
    std::string                           getTargetPath() const;
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

    recvStatus                            getRecvStatus() const;
    int                                   getContentLength() const;
    HttpStatus                            getErrorStatus() const;
    bool                                  isError() const;

    void                                  setBody(const std::string& body);

    void                                  setRecvStatus(recvStatus status);
    void                                  setContentLength(int len);
    void                                  setErrorStatus(HttpStatus status);
    void                                  setCgi(bool cgi);

    void                                  unchunk();



  private:
    HttpRequest(const HttpRequest& obj);

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

    recvStatus                            rs;
    int                                   contentLength;
    HttpStatus                            errorStatus;

    void                                  parseStatusLine(const std::string &line);

    void                                  setURI(const std::string& URI);
    void                                  setMethod(const std::string& method);
    void                                  setVersion(const std::string& version);

    void                                  validateMethod(const std::string &method);
    void                                  validateVersion(const std::string &path);
    void                                  validateURI(const std::string &version);
    unsigned int                          strToHex(std::string s);
};

#endif
