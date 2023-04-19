#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "Http.hpp"
# include <stdexcept>
#include <string>
#include <vector>
namespace request_method {
  const std::string GET = "GET";
  const std::string POST = "POST";
  const std::string DELETE = "DELETE";
}

namespace request_field {
  const std::string CACHE_CONTROL = "Cache-control";
  const std::string HOST = "Host";
  const std::string PRAGMA = "Pragma";
  const std::string RANGE = "Range";
  const std::string TE = "TE";
  const std::string EXPECT = "Expect";
  const std::string MAX_FORWARDS = "Max-forwards";

  const std::string IF_MATCH = "If-Match";
  const std::string IF_NONE_MATCH = "If-None-Match";
  const std::string IF_MODIFIED_SINCE = "If-Modified-Since";
  const std::string IF_UNMODIFIED_SINCE = "If-Unmodified-Since";
  const std::string IF_RANGE = "If-Range";

  const std::string ACCEPT = "Accept";
  const std::string ACCEPT_CHARSET = "Accept-Charset";
  const std::string ACCEPT_ENCODING = "Accept-Encoding";
  const std::string ACCEPT_LANGUAGE = "Accept-Language";

  const std::string AUTHORIZATION = "Authorization";
  const std::string PROXY_AUTHORIZATION = "Proxy-Authorization";

  const std::string FROM = "From";
  const std::string REFERER = "Referer";
  const std::string USER_AGENT = "User-Agent";
}
void zz() {};

std::map<std::string, void (*)()> test = { {request_field::CACHE_CONTROL, zz}
}


class HttpRequest : public Http {
  public:
    HttpRequest(const std::string& data);

    std::string getMethod() const;
    std::string getPath() const;
    std::string getVersion() const;

  private:
    std::map<std::string, void (*)(const std::string &)> fields;
    std::string method;
    std::string path;
    std::string version;

    void validateMethod(const std::string &method);
    void validateVersion(const std::string &path);
    void validatePath(const std::string &version);

    void parseStatusLine(const std::string &line);
    void parseRequest(const std::string &s);

    static void parseCacheControl(const std::string &s);
    static void parseHost(const std::string &s);
    static void parsePragma(const std::string &s);
    static void parseRange(const std::string &s);
    static void parseTE(const std::string &s);
    static void parseExpect(const std::string &s);
    static void parseMaxForwards(const std::string &s);
    static void parseIfMatch(const std::string &s);
    static void parseIfNoneMatch(const std::string &s);
    static void parseIfModifiedSince(const std::string &s);
    static void parseIfUnmodifiedSince(const std::string &s);
    static void parseIfRange(const std::string &s);

    static void parseAccept(const std::string &s);
    static void parseAcceptCharset(const std::string &s);
    static void parseAcceptEncoding(const std::string &s);
    static void parseAcceptLanguage(const std::string &s);

    static void parseAuthorization(const std::string &s);
    static void parseProxyAuthorization(const std::string &s);
    static void parseFrom(const std::string &s);
    static void parseReferer(const std::string &s);
    static void parseUserAgent(const std::string &s);
};

#endif
