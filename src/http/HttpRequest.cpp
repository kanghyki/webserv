#include "./HttpRequest.hpp"

HttpRequest::HttpRequest(std::string request): errorFlag(false), errorStatus(OK) {
  std::vector<std::string> ss = util::split(request, "\r\n");
  parseStatusLine(ss[0]);

  //  for (std::vector<std::string>::iterator it = ss.begin(); it != ss.end(); ++it) {
  //    if (*it == "\r\n") {
  //      break ;
  //    }
  //    std::vector<std::string> itss = util::split(*it, ":");
  //    if (itss.size() != 2) {
  //      this->errorFlag = true;
  //      this->errorStatus = 400;
  //    }
  //    this->header.insert(std::make_pair(itss[0], itss[1]));
  //  }
}

HttpRequest::~HttpRequest() {}

void HttpRequest::validateMethod(const std::string &method) {
  if (method == request_method::GET
      || method == request_method::POST
      || method == request_method::DELETE) {
    this->method = method;
  }
  else
    setError(METHOD_NOT_ALLOWED);
}

void HttpRequest::validatePath(const std::string &path) {
  if (path.length() > 2000)
    setError(URI_TOO_LONG);
  for (int i = 0; i < path.length(); ++i) {
    if (!std::isalpha(path[i])
        && !std::isdigit(path[i])
        && !std::strchr(":%._\\+~#?&/=", path[i]))
      setError(BAD_REQUEST);
  }
}

void HttpRequest::validateVersion(const std::string &version) {
  char *left;
  double v;
  std::vector<std::string> ss = util::split(version, "/");

  if (ss.size() != 2 || ss[0] != "HTTP")
    setError(BAD_REQUEST);
  v = std::strtod(ss[1].c_str(), &left);
  if (std::strlen(left) > 0)
    setError(BAD_REQUEST);
  if (v > 1.1)
    setError(HTTP_VERSION_NOT_SUPPORTED);
  if (v < 1.1)
    setError(UPGRADE_REQUIRED);
}

void HttpRequest::parseStatusLine(const std::string& line) {
  std::vector<std::string> ss = util::split(line, " ");
  if (ss.size() != 3)
    setError(BAD_REQUEST);

  validateMethod(ss[0]);
  validatePath(ss[1]);
  validateVersion(ss[2]);

  this->method = ss[0]; 
  this->path = ss[1];
  this->version = ss[2];
}

/*
 * -------------------------- Getter -------------------------------
 */

std::string HttpRequest::getMethod() const {
  return this->method;
}

std::string HttpRequest::getPath() const {
  return this->path;
}

std::string HttpRequest::getVersion() const {
  return this->version;
}

bool HttpRequest::isError() const {
  return this->errorFlag;
}

bool HttpRequest::getErrorStatus() const {
  return this->errorStatus;
}

/*
 * -------------------------- Setter -------------------------------
 */

void HttpRequest::setError(HttpStatus errorStatus) {
  this->errorFlag = true;
  this->errorStatus = errorStatus;
}

//void HttpRequest::parseCacheControl(const std::string &s) {
//}
//
//void HttpRequest::parseHost(const std::string &s) {
//}
//
//void HttpRequest::parsePragma(const std::string &s) {
//}
//
//void HttpRequest::parseRange(const std::string &s) {
//}
//
//void HttpRequest::parseTE(const std::string &s) {
//}
//
//void HttpRequest::parseExpect(const std::string &s) {
//}
//
//void HttpRequest::parseMaxForwards(const std::string &s) {
//}
//
//void HttpRequest::parseIfMatch(const std::string &s) {
//}
//
//void HttpRequest::parseIfNoneMatch(const std::string &s) {
//}
//
//void HttpRequest::parseIfModifiedSince(const std::string &s) {
//}
//
//void HttpRequest::parseIfUnmodifiedSince(const std::string &s) {
//}
//
//void HttpRequest::parseIfRange(const std::string &s) {
//}
//
//void HttpRequest::parseAccept(const std::string &s) {
//}
//
//void HttpRequest::parseAcceptCharset(const std::string &s) {
//}
//
//void HttpRequest::parseAcceptEncoding(const std::string &s) {
//}
//
//void HttpRequest::parseAcceptLanguage(const std::string &s) {
//}
//
//void HttpRequest::parseAuthorization(const std::string &s) {
//}
//
//void HttpRequest::parseProxyAuthorization(const std::string &s) {
//}
//
//void HttpRequest::parseFrom(const std::string &s) {
//}
//
//void HttpRequest::parseReferer(const std::string &s) {
//}
//
//void HttpRequest::parseUserAgent(const std::string &s) {
//}
