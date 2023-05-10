#include "HttpResponse.hpp"

const std::string HttpResponse::version = "HTTP/1.1";

/*
 * -------------------------- Constructor --------------------------
 */

HttpResponse::HttpResponse():
  statusCode(INTERNAL_SERVER_ERROR),
  statusText(""),
  header(),
  body(""),
  buffer(""),
  buffer_size(0),
  sendLength(0) {
}

HttpResponse::HttpResponse(const HttpResponse& obj):
  statusCode(obj.statusCode),
  statusText(obj.statusText),
  header(obj.header),
  body(obj.body),
  buffer(obj.buffer),
  buffer_size(obj.buffer_size),
  sendLength(obj.sendLength) {
}

HttpResponse& HttpResponse::operator=(const HttpResponse& obj) {
  if (this != &obj) {
    this->statusCode = obj.statusCode;
    this->statusText = obj.statusText;
    this->header = obj.header;
    this->body = obj.body;

    this->buffer = obj.buffer;
    this->buffer_size = obj.buffer_size;
    this->sendLength = obj.sendLength;
  }

  return *this;
}

/*
 * -------------------------- Destructor ---------------------------
 */

HttpResponse::~HttpResponse() {}

/*
 * -------------------------- Getter -------------------------------
 */

HttpResponseHeader& HttpResponse::getHeader() {
  return this->header;
}

HttpStatus HttpResponse::getStatusCode() const {
  return this->statusCode;
}

HttpResponse::SendStatus HttpResponse::getSendStatus() const {
  if (this->buffer_size == this->sendLength)
    return DONE;
  return SENDING;
}

/*
 * -------------------------- Setter -------------------------------
 */

void HttpResponse::addSendLength(unsigned int length) {
  this->sendLength += length;
}

void HttpResponse::setStatusCode(const HttpStatus statusCode) {
  this->statusCode = statusCode;
}

void HttpResponse::setBody(const std::string& body) {
  this->body = body;
}

void HttpResponse::removeBody() {
  this->body = "";
}

std::string HttpResponse::toString() throw() {
  std::string ret;

  if (this->buffer.empty() == false)
    return this->buffer.substr(this->sendLength);

  this->statusText = getStatusText(this->statusCode);
  this->header.set(HttpResponseHeader::CONTENT_LENGTH, util::itoa(body.length()));
  this->header.set(HttpResponseHeader::DATE, getCurrentTimeStr());
  this->header.set(HttpResponseHeader::SERVER, "webserv/1.0");
  ret = makeStatusLine();
  ret += this->header.toStringForResponse();
  ret += "\r\n";
  ret += this->body;

  this->buffer = ret;
  this->buffer_size = ret.length();

  return ret;
}

std::string HttpResponse::makeStatusLine() const {
  return this->version + " " + util::itoa(this->statusCode) + " " + this->statusText + "\r\n";
}

// TODO: move
std::string HttpResponse::getCurrentTimeStr() const {
  // example: "Date: Sat, 9 Jul 2023 13:12:42 UTC"
  time_t curr_time;
  char buf[100];

  curr_time = time(NULL);
  tm *time = gmtime(&curr_time);
  strftime(buf, 100, "%a, %d %b %Y %X GMT", time);

  return buf;
}
