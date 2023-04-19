#include "HttpRequest.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

HttpRequest::HttpRequest(const std::string& data) {
//  parseRequest(getStartLine(data));
  this->fieldMethod.insert(std::make_pair(request_field::CACHE_CONTROL, HttpRequest::parseCacheControl));
  this->fieldMethod.insert(std::make_pair(request_field::HOST, HttpRequest::parseHost));
  this->fieldMethod.insert(std::make_pair(request_field::PRAGMA, HttpRequest::parsePragma));
  this->fieldMethod.insert(std::make_pair(request_field::RANGE, HttpRequest::parseRange));
  this->fieldMethod.insert(std::make_pair(request_field::TE, HttpRequest::parseTE));
  this->fieldMethod.insert(std::make_pair(request_field::EXPECT, HttpRequest::parseExpect));
  this->fieldMethod.insert(std::make_pair(request_field::MAX_FORWARDS, HttpRequest::parseMaxForwards));
  this->fieldMethod.insert(std::make_pair(request_field::IF_MATCH, HttpRequest::parseIfMatch));
  this->fieldMethod.insert(std::make_pair(request_field::IF_NONE_MATCH, HttpRequest::parseIfNoneMatch));
  this->fieldMethod.insert(std::make_pair(request_field::IF_MODIFIED_SINCE, HttpRequest::parseIfModifiedSince));
  this->fieldMethod.insert(std::make_pair(request_field::IF_UNMODIFIED_SINCE, HttpRequest::parseIfUnmodifiedSince));
  this->fieldMethod.insert(std::make_pair(request_field::IF_RANGE, HttpRequest::parseIfRange));
  this->fieldMethod.insert(std::make_pair(request_field::ACCEPT, HttpRequest::parseAccept));
  this->fieldMethod.insert(std::make_pair(request_field::ACCEPT_CHARSET, HttpRequest::parseAcceptCharset));
  this->fieldMethod.insert(std::make_pair(request_field::ACCEPT_ENCODING, HttpRequest::parseAcceptEncoding));
  this->fieldMethod.insert(std::make_pair(request_field::ACCEPT_LANGUAGE, HttpRequest::parseAcceptLanguage));
  this->fieldMethod.insert(std::make_pair(request_field::AUTHORIZATION, HttpRequest::parseAuthorization));
  this->fieldMethod.insert(std::make_pair(request_field::PROXY_AUTHORIZATION, HttpRequest::parseProxyAuthorization));
  this->fieldMethod.insert(std::make_pair(request_field::FROM, HttpRequest::parseFrom));
  this->fieldMethod.insert(std::make_pair(request_field::REFERER, HttpRequest::parseReferer));
  this->fieldMethod.insert(std::make_pair(request_field::USER_AGENT, HttpRequest::parseUserAgent));
}

/*
 * -------------------------- Destructor ---------------------------
 */

/*
 * -------------------------- Operator -----------------------------
 */

/*
 * -------------------------- Getter -------------------------------
 */

std::string HttpRequest::getMethod() const { return this->method; }

std::string HttpRequest::getPath() const { return this->path; }

std::string HttpRequest::getVersion() const { return this->version; }

/*
 * -------------------------- Setter -------------------------------
 */

/*
 * ----------------------- Member Function -------------------------
 */

void HttpRequest::validateMethod(const std::string &method) {
  if (method == request_method::GET
      || method == request_method::POST
      || method == request_method::DELETE)
    throw std::invalid_argument("request error");
}

void HttpRequest::validatePath(const std::string &path) {
  if (path.length() > 2000) {
    //
    throw std::invalid_argument("414 url too long");
  }
  for (int i = 0; i < path.length(); ++i) {
    if (!std::isalpha(path[i])
        && !std::isdigit(path[i])
        && !std::strchr(":%._\\+~#?&/=", path[i]))
      throw std::invalid_argument("request error");
  }
}

void HttpRequest::validateVersion(const std::string &version) {
  char *left;
  double v;
  std::vector<std::string> ss = util::split(version, "/");

  if (ss.size() != 2)
    throw std::invalid_argument("version error");

  if (ss[0] == "HTTP")
    throw std::invalid_argument("version error");

  v = std::strtod(ss[1].c_str(), &left);
  if (std::strlen(left) > 0) {
    throw std::invalid_argument("version error");
  }
//  if (v > 1.1) {
//    throw std::invalid_argument("505 http version not supported");
//  }
//  if (v < 1.1) {
//    throw std::invalid_argument("426 Upgrade Required");
//  }
}

void HttpRequest::parseStatusLine(const std::string& line) {
  std::vector<std::string> ss = util::split(line, " ");

  if (ss.size() != 3)
    throw except::HttpRequestException();
  // TODO: validate

  validateMethod(ss[0]);
  this->method = ss[0]; 

  validatePath(ss[1]);
  this->path = ss[1];

  // TODO: validate
  validateVersion(ss[2]);
  this->version = ss[2];
}

void HttpRequest::parseRequest(const std::string &s) {
  std::vector<std::string> ss = util::split(s, "\n");

  for (std::vector<std::string>::iterator it = ss.begin(); it != ss.end(); ++it) {
    std::vector<std::string> itss = util::split(*it, ":");

    if (itss.size() != 2) {
      throw std::invalid_argument("error");
    }
    std::map<std::string, void (*)(const std::string &)>::iterator fd
          = this->fieldMethod.find(itss[0]);
    if (this->fieldMethod.end() != fd)
      fd->second(itss[1]);
  }
}

/*
 * ---------------------- Non-Member Function ----------------------
 */


/*
 * ---------------------- parse Function ----------------------
 */

void HttpRequest::parseCacheControl(const std::string &s) {
}

void HttpRequest::parseHost(const std::string &s) {
}

void HttpRequest::parsePragma(const std::string &s) {
}

void HttpRequest::parseRange(const std::string &s) {
}

void HttpRequest::parseTE(const std::string &s) {
}

void HttpRequest::parseExpect(const std::string &s) {
}

void HttpRequest::parseMaxForwards(const std::string &s) {
}

void HttpRequest::parseIfMatch(const std::string &s) {
}

void HttpRequest::parseIfNoneMatch(const std::string &s) {
}

void HttpRequest::parseIfModifiedSince(const std::string &s) {
}

void HttpRequest::parseIfUnmodifiedSince(const std::string &s) {
}

void HttpRequest::parseIfRange(const std::string &s) {
}

void HttpRequest::parseAccept(const std::string &s) {
}

void HttpRequest::parseAcceptCharset(const std::string &s) {
}

void HttpRequest::parseAcceptEncoding(const std::string &s) {
}

void HttpRequest::parseAcceptLanguage(const std::string &s) {
}

void HttpRequest::parseAuthorization(const std::string &s) {
}

void HttpRequest::parseProxyAuthorization(const std::string &s) {
}

void HttpRequest::parseFrom(const std::string &s) {
}

void HttpRequest::parseReferer(const std::string &s) {
}

void HttpRequest::parseUserAgent(const std::string &s) {
}

