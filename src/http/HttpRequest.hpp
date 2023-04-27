#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include "../config/ServerConfig.hpp"
# include "./HttpHeaderField.hpp"
# include "./HttpStatus.hpp"
# include "../Util.hpp"

# include <stdexcept>
# include <string>
# include <vector>
# include <map>

namespace request_method {
  const std::string GET    = "GET";
  const std::string POST   = "POST";
  const std::string DELETE = "DELETE";
  const std::string PUT    = "PUT";
}

class HttpRequest {
  public:
    HttpRequest(std::string request, const ServerConfig& sc);
    ~HttpRequest();

    void                                parseHeader(const std::string &h) throw(HttpStatus);

    std::string                         getMethod() const;
    std::string                         getPath() const;
    std::string                         getQueryString() const;
    std::string                         getVersion() const;
    std::string                         getField(const std::string& field) const;
    std::string                         getBody() const;
    const std::string                   getContentType(void) const;
    const LocationConfig&               getConfig() const;

    void                                setBody(const std::string& body);

  private:
    static const size_t                 URL_MAX_LENGTH;
    static const std::string            CRLF;

    std::string                         method;
    std::string                         path;
    std::string                         queryString;
    std::string                         version;
    std::string                         body;
    std::map<std::string, std::string>  field;
    LocationConfig                      config;

    void                                setURI(const std::string& URI);
    void                                setMethod(const std::string& method);
    void                                setVersion(const std::string& version);

    void                                parseStatusLine(const std::string &line);

    void                                validateMethod(const std::string &method);
    void                                validateVersion(const std::string &path);
    void                                validateURI(const std::string &version);

    std::pair<std::string, std::string> splitField(const std::string& line);

    const std::string                   getMimeType(void) const;

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
