#include "HttpResponse.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

HttpResponse::HttpResponse() {}

/*
 * -------------------------- Destructor ---------------------------
 */

HttpResponse::~HttpResponse() {}

/*
 * -------------------------- Setter -------------------------------
 */

void HttpResponse::setStatusCode(const HttpStatus statusCode) {
  this->statusCode = statusCode;
  this->statusText = getStatusText(statusCode);
}

void HttpResponse::setHttpVersion(const std::string& version) {
  this->version = version;
}

void HttpResponse::setHeader(const std::string& field, const std::string& value) {
  this->header.insert(std::make_pair(field, value));
}

void HttpResponse::setBody(const std::string& body) {
  setHeader("Content-length", std::to_string(body.length()));
  this->body = body;
}

std::string HttpResponse::toString() {
  std::string ret;

  ret = this->version
    + " "
    + std::to_string(this->statusCode)
    + " "
    + this->statusText
    + "\r\n";
  for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it)
    ret += (*it).first + ": " + (*it).second + "\r\n";
  if (this->body.size() > 0)
    ret += "\r\n" + this->body;

  return ret;
}

// TODO: move
std::string getNowStr() {
  // example: "Date: Sat, 9 Jul 2023 13:12:42 GMT"
  time_t curr_time;
  curr_time = time(NULL);
  char buf[100];

  tm *time = gmtime(&curr_time);
  strftime(buf, 100, "%a, %d %b %Y %X %Z", time);
  return buf;
}
