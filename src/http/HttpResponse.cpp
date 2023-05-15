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
  isSetBuffer(false),
  buffer(""),
  buffer_size(0),
  sendLength(0),
  cgi_stat(NOT_CGI),
  cgi(),
  status(-1),
  writeFd(-1),
  readFd(-1) {
}

HttpResponse::HttpResponse(const HttpResponse& obj):
  statusCode(obj.statusCode),
  statusText(obj.statusText),
  header(obj.header),
  body(obj.body),
  isSetBuffer(obj.isSetBuffer),
  buffer(obj.buffer),
  buffer_size(obj.buffer_size),
  sendLength(obj.sendLength),
  cgi_stat(obj.cgi_stat),
  cgi(obj.cgi),
  status(obj.status),
  writeFd(obj.writeFd),
  readFd(obj.readFd),
  fd(obj.fd),
  autoindex(obj.autoindex),
  method(obj.method) {
}

HttpResponse& HttpResponse::operator=(const HttpResponse& obj) {
  if (this != &obj) {
    this->statusCode = obj.statusCode;
    this->statusText = obj.statusText;
    this->header = obj.header;
    this->body = obj.body;

    this->isSetBuffer = obj.isSetBuffer;
    this->buffer = obj.buffer;
    this->buffer_size = obj.buffer_size;
    this->sendLength = obj.sendLength;

    this->cgi_stat = obj.cgi_stat;
    this->cgi = obj.cgi;

    this->status = obj.status;
    this->writeFd = obj.writeFd;
    this->readFd = obj.readFd;
    this->fd = obj.fd;

    this->autoindex = obj.autoindex;
    this->method = obj.method;
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
  if (this->isSetBuffer == true && this->buffer_size == this->sendLength)
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

  if (this->isSetBuffer == true)
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
  this->isSetBuffer = true;

  return ret;
}

std::string HttpResponse::makeStatusLine() const {
  return this->version + " " + util::itoa(this->statusCode) + " " + this->statusText + "\r\n";
}

std::string HttpResponse::getCurrentTimeStr() const {
  // example: "Date: Sat, 9 Jul 2023 13:12:42 UTC"
  time_t curr_time;
  char buf[100];

  curr_time = time(NULL);
  tm *time = gmtime(&curr_time);
  strftime(buf, 100, "%a, %d %b %Y %X GMT", time);

  return buf;
}

void HttpResponse::set_cgi_status(HttpResponse::cgi_status s) {
  this->cgi_stat = s;
}

HttpResponse::cgi_status HttpResponse::get_cgi_status() const {
  return this->cgi_stat;
}

CGI& HttpResponse::getCGI() {
  return this->cgi;
}

int HttpResponse::getStatus() const {
  return this->status;
}

int HttpResponse::getWriteFd() const {
  return this->writeFd;
}
int HttpResponse::getReadFd() const {
  return this->readFd;
}

void HttpResponse::openToWrite(const std::string& fileName) {
  int fd;

  if ((fd = open(fileName.c_str(), O_WRONLY)) == -1)
    throw util::SystemFunctionException();
  fcntl(fd, F_SETFL, O_NONBLOCK);
  this->fd = fd;
}

void HttpResponse::openToRead(const std::string& fileName) {
  int fd;

  if ((fd = open(fileName.c_str(), O_RDONLY)) == -1)
    throw util::SystemFunctionException();
  fcntl(fd, F_SETFL, O_NONBLOCK);
  this->fd = fd;
}

void HttpResponse::setAutoIndex(bool autoindex) {
  this->autoindex = autoindex;
}

bool HttpResponse::isAutoindex() const {
  return this->autoindex;
}

void HttpResponse::setMethod(const std::string& method) {
  this->method = method;
}

std::string HttpResponse::getMethod(void) const {
  return this->method;
}

int HttpResponse::getFd() const {
  return this->fd;
}
