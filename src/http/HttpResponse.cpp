#include "HttpResponse.hpp"

const std::string HttpResponse::version = "HTTP/1.1";

/*
 * -------------------------- Constructor --------------------------
 */

HttpResponse::HttpResponse(): statusCode(INTERNAL_SERVER_ERROR), statusText() {}

/*
 * -------------------------- Destructor ---------------------------
 */

HttpResponse::~HttpResponse() {}

/*
 * -------------------------- Setter -------------------------------
 */

void HttpResponse::setStatusCode(const HttpStatus statusCode) {
  this->statusCode = statusCode;
}

void HttpResponse::setHeader(const std::string& field, const std::string& value) {
  this->header.insert(std::make_pair(field, value));
}

void HttpResponse::setBody(const std::string& body) {
  this->body = body;
}

std::string HttpResponse::toString() {
  std::string ret;

  this->statusText = getStatusText(this->statusCode);
  setHeader(request_field::CONTENT_LENGTH, util::itoa(body.length()));
  ret = makeStatusLine();
  for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it)
    ret += makeHeaderField(it->first, it->second);
  ret += "\r\n";
  ret += this->body;

  return ret;
}

std::string HttpResponse::makeStatusLine() const {
  return this->version + " " + util::itoa(this->statusCode) + " " + this->statusText + "\r\n";
}

std::string HttpResponse::makeHeaderField(const std::string& fieldName, const std::string& value) const {
  return fieldName + ": " + value + "\r\n";
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
