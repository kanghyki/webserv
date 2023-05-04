#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include "./HttpHeaderField.hpp"
# include "./HttpStatus.hpp"
# include "./HttpHeaderField.hpp"
# include "../Util.hpp"
# include "../MimeType.hpp"
# include "../config/Config.hpp"

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


    void                                  parse(std::string request, const Config& conf);
    void                                  parseHeader(const std::string &h) throw(HttpStatus);
    void                                  setConfig(const Config& conf);
    void                                  checkCGI(const std::string& path, const ServerConfig& sc);
    

    std::string                           getMethod() const;
    std::string                           getPath() const;
    std::string                           getRelativePath() const;
    std::string                           getQueryString() const;
    std::string                           getVersion() const;
    std::string                           getField(const std::string& field) const;
    std::string                           getBody() const;
    const std::string                     getContentType(void) const;
    const LocationConfig&                 getLocationConfig() const;
    const ServerConfig&                   getServerConfig() const;
    bool                                  isCGI() const;
    const std::string                     getScriptPath() const;
    const std::string                     getCGIPath() const;
    const std::string                     getPathInfo() const;
    std::string                           getRecvData() const;
    int                                   getRecvStatus() const;
    int                                   getContentLength() const;


    void                                  setBody(const std::string& body);
    void                                  setRecvData(const std::string& data);
    void                                  addRecvData(const std::string& data);
    void                                  clearRecvData();
    void                                  setRecvStatus(int status);
    void                                  setContentLength(int len);

//    HttpRequest& operator=(const HttpRequest& obj);


  private:
    HttpRequest(const HttpRequest& obj);

    static const size_t                   URL_MAX_LENGTH;

//    std::string                           request;
    std::string                           method;
    std::string                           path;
    std::string                           queryString;
    std::string                           version;
    std::string                           body;
    std::map<std::string, std::string>    field;
    ServerConfig                          sc;
    LocationConfig                        lc;
    bool                                  cgi;
    std::string                           scriptPath;
    std::string                           cgiPath;
    std::string                           pathInfo;
    std::string                           recvData;
    int                                   recvStatus;
    int                                   contentLength;

    void                                  parseStatusLine(const std::string &line);

    void                                  setURI(const std::string& URI);
    void                                  setMethod(const std::string& method);
    void                                  setVersion(const std::string& version);

    void                                  validateMethod(const std::string &method);
    void                                  validateVersion(const std::string &path);
    void                                  validateURI(const std::string &version);

    std::pair<std::string, std::string>   splitField(const std::string& line);

//    std::map<std::string, void (*)(const std::string &)> fieldMethod;

//    static void parseCacheControl(const std::string &s);
//    static void parseHost(const std::string &s);
//    static void parsePragma(const std::string &s);
//    static void parseRange(const std::string &s);
//    static void parseTE(const std::string &s);
//    static void parseExpect(const std::string &s);
//    static void parseMaxForwards(const std::string &s);
//    static void parseIfMatch(const std::string &s);
//    static void parseIfNoneMatch(const std::string &s);
//    static void parseIfModifiedSince(const std::string &s);
//    static void parseIfUnmodifiedSince(const std::string &s);
//    static void parseIfRange(const std::string &s);
//
//    static void parseAccept(const std::string &s);
//    static void parseAcceptCharset(const std::string &s);
//    static void parseAcceptEncoding(const std::string &s);
//    static void parseAcceptLanguage(const std::string &s);
//
//    static void parseAuthorization(const std::string &s);
//    static void parseProxyAuthorization(const std::string &s);
//    static void parseFrom(const std::string &s);
//    static void parseReferer(const std::string &s);
//    static void parseUserAgent(const std::string &s);
};

#endif
