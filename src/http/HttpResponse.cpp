#include "HttpResponse.hpp"
#include "HttpHeaderField.hpp"

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

HttpStatus HttpResponse::getStatusCode() const {
  return this->statusCode;
}

HttpResponse::sendStatus HttpResponse::getSendStatus() const {
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

void HttpResponse::addHeader(const std::string& field, const std::string& value) {
  this->header.insert(std::make_pair(field, value));
}

void HttpResponse::removeHeader(const std::string& field) {
  std::map<std::string, std::string>::const_iterator target;

  target = this->header.end();
  // FIXME: case consistency
  for (std::map<std::string, std::string>::const_iterator it = this->header.begin(); it != this->header.end(); ++it) {
    if (util::toLowerStr(it->first) == util::toLowerStr(field)) {
      target = it;
      break;
    }
  }
  if (target != this->header.end())
    this->header.erase(target);
}

std::string HttpResponse::toString() throw() {
  std::string ret;

  if (this->buffer.empty() == false)
    return this->buffer.substr(this->sendLength);

  this->statusText = getStatusText(this->statusCode);
  addHeader(header_field::CONTENT_LENGTH, util::itoa(body.length()));
  addHeader("Date", getCurrentTimeStr());
  ret = makeStatusLine();
  for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it)
    ret += makeHeaderField(it->first, it->second);
  ret += "\r\n";
  ret += this->body;

  this->buffer = ret;
  this->buffer_size = ret.length();

  return ret;
}

std::string HttpResponse::makeStatusLine() const {
  return this->version + " " + util::itoa(this->statusCode) + " " + this->statusText + "\r\n";
}

std::string HttpResponse::makeHeaderField(const std::string& fieldName, const std::string& value) const {
  return fieldName + ": " + value + "\r\n";
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
