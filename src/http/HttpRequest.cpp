#include "./HttpRequest.hpp"
#include <iostream>

const size_t      HttpRequest::URL_MAX_LENGTH = 2000;
const std::string HttpRequest::CRLF = "\r\n";

//HttpRequest::HttpRequest(std::string request) {
//  std::vector<std::string>            vs;
//
//  vs = util::split(request, CRLF + CRLF);
//  parseHeader(vs[0]);
//  this->body = vs[1];
//}

HttpRequest::HttpRequest() {}

HttpRequest::~HttpRequest() {}

void HttpRequest::parseHeader(const std::string& h) {
  std::vector<std::string>            vs;
  std::vector<std::string>::iterator  it;

  vs = util::split(h, CRLF);
  it = vs.begin();
  parseStatusLine(*it);
  while (++it != vs.end()) {
    std::pair<std::string, std::string> ret = splitField(*it);
    this->field.insert(ret);
  }
}

void HttpRequest::parseStatusLine(const std::string& line) {
  std::vector<std::string> vs = util::split(line, " ");
  if (vs.size() != 3) throw BAD_REQUEST;

  validateMethod(vs[0]);
  validatePath(vs[1]);
  validateVersion(vs[2]);

  this->method = vs[0]; 
  this->path = vs[1];
  this->version = vs[2];
}

void HttpRequest::validateMethod(const std::string &method) {
  if (method != request_method::GET && method != request_method::POST && method != request_method::DELETE)
    throw METHOD_NOT_ALLOWED;
  this->method = method;
}

void HttpRequest::validatePath(const std::string &path) {
  int i;

  i = 0;
  if (path.length() > URL_MAX_LENGTH)     throw URI_TOO_LONG;
  if (path[i++] != '/')                   throw BAD_REQUEST;

  while (i < path.length()) {
    if (!std::isalnum(path[i]) && !std::strchr(":%._\\+~#?&/=", path[i]))
      throw BAD_REQUEST;
    ++i;
  }
}

void HttpRequest::validateVersion(const std::string &version) {
  char*                                   left;
  double                                  v;
  std::vector<std::string>                ss = util::split(version, "/");

  if (ss.size() != 2 || ss[0] != "HTTP")  throw BAD_REQUEST;
  v = std::strtod(ss[1].c_str(), &left);
  if (std::strlen(left) > 0)              throw BAD_REQUEST;
  if (v > 1.1)                            throw HTTP_VERSION_NOT_SUPPORTED;
  if (v < 1.1)                            throw UPGRADE_REQUIRED;
}

std::pair<std::string, std::string> HttpRequest::splitField(const std::string& line) {
  std::string field;
  std::string value;
  int pos;

  if ((pos = line.find(":")) == std::string::npos) throw BAD_REQUEST;
  field = util::trimSpace(line.substr(0, pos));
  value = util::trimSpace(line.substr(pos + 1));

  return std::make_pair(field, value);
}

// getter

std::string HttpRequest::getMethod() const { return this->method; }

std::string HttpRequest::getPath() const { return this->path; }

std::string HttpRequest::getVersion() const { return this->version; }

std::string HttpRequest::getField(const std::string& field) const {
  std::map<std::string, std::string>::const_iterator it;
  std::string                                        ret;

  if ((it = this->field.find(field)) != this->field.end())
    ret = it->second;

  return ret;
}

std::string HttpRequest::getBody() const {
  return this->body;
}

// setter

void  HttpRequest::setBody(const std::string& body) { this->body = body; }

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
