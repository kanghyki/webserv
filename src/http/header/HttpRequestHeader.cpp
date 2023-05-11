#include "./HttpRequestHeader.hpp"

const std::string HttpRequestHeader::HOST = "Host";
const std::string HttpRequestHeader::TRANSFER_ENCODING = "Transfer-Encoding";
const std::string HttpRequestHeader::CONNECTION = "Connection";
const std::string HttpRequestHeader::CONTENT_TYPE = "Content-Type";
const std::string HttpRequestHeader::CONTENT_LENGTH = "Content-Length";
const std::string HttpRequestHeader::COOKIE = "Cookie";

HttpRequestHeader::HttpRequestHeader():
  conn(KEEP_ALIVE),
  te(UNSET) {
}

HttpRequestHeader::~HttpRequestHeader() {
}

HttpRequestHeader& HttpRequestHeader::operator=(const HttpRequestHeader& obj) {
  if (this != &obj) {
    this->header = obj.header;
    this->conn = obj.conn;
    this->te = obj.te;
  }

  return *this;
}

void HttpRequestHeader::parse(const std::string& reqHeader) {
  std::vector<std::string>            vs;
  std::vector<std::string>::iterator  it;

  vs = util::split(reqHeader, CRLF);
  for (it = vs.begin(); it != vs.end(); ++it) {
    std::pair<std::string, std::string> p = splitField(*it);
    this->header.set(p.first, p.second);
  }
  parseConnection();
  parseTransferEncoding();
}

void HttpRequestHeader::parseConnection() {
  std::string s = util::toLowerStr(get(CONNECTION));

  if (s == "keep-alive")
    setConnection(KEEP_ALIVE);
  else if (s == "close")
    setConnection(CLOSE);
}

void HttpRequestHeader::parseTransferEncoding() {
  std::string s = util::toLowerStr(get(TRANSFER_ENCODING));
  if (s == "chunked")
    setTransferEncoding(CHUNKED);
}

std::pair<std::string, std::string> HttpRequestHeader::splitField(std::string line) {
  std::string field;
  std::string value;
  size_t pos;

  if ((pos = line.find(":")) == std::string::npos) throw BAD_REQUEST;
  field = util::trimSpace(line.substr(0, pos));
  value = util::trimSpace(line.substr(pos + 1));

  return std::make_pair(field, value);
}

// getter

std::string HttpRequestHeader::get(std::string key) const {
  return this->header.get(key);
}

HttpRequestHeader::transfer_encoding HttpRequestHeader::getTransferEncoding() const {
  return this->te;
}

HttpRequestHeader::connection HttpRequestHeader::getConnection() const {
  return this->conn;
}

const std::map<std::string, std::string>  HttpRequestHeader::getCustomeHeader() const {
  std::map<std::string, std::string>  ret;
  std::map<std::string, std::string>  hd;

  hd = this->header.getCopy();
  std::map<std::string, std::string>::const_iterator it = hd.begin();
  for (; it != hd.end(); ++it) {
    if (it->first[0] == 'x')
      ret.insert(std::pair<std::string, std::string>(it->first, it->second));
  }

  return ret;
}

// setter

void  HttpRequestHeader::setConnection(HttpRequestHeader::connection conn) {
  this->conn = conn;
}

void HttpRequestHeader::setTransferEncoding(HttpRequestHeader::transfer_encoding te) {
  this->te = te;
}
