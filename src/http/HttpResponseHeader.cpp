#include "./HttpResponseHeader.hpp"

const std::string HttpResponseHeader::CONTENT_TYPE = "content-type";
const std::string HttpResponseHeader::CONTENT_LENGTH = "content-length";
const std::string HttpResponseHeader::DATE = "date";
const std::string HttpResponseHeader::KEEP_ALIVE = "keep-alive";
const std::string HttpResponseHeader::LOCATION = "location";
const std::string HttpResponseHeader::ALLOW = "allow";
const std::string HttpResponseHeader::SERVER = "server";
const std::string HttpResponseHeader::CONNECTION = "connection";
const std::string HttpResponseHeader::UPGRADE = "upgrade";
const std::string HttpResponseHeader::SET_COOKIE = "set-cookie";

HttpResponseHeader::HttpResponseHeader() {}

HttpResponseHeader::HttpResponseHeader(const HttpResponseHeader& obj):
  header(obj.header) {}

HttpResponseHeader::~HttpResponseHeader() {}

HttpResponseHeader& HttpResponseHeader::operator=(const HttpResponseHeader& obj) {
  if (this != &obj) {
    this->header = obj.header;
  }

  return *this;
}

void HttpResponseHeader::set(const std::string& key, const std::string& value) {
  std::string lkey;

  lkey = util::toLowerStr(key);
  this->header.insert(std::make_pair(lkey, value));
}

std::string HttpResponseHeader::get(const std::string& key) const {
  std::map<std::string, std::string>::const_iterator it;

  it = this->header.find(key);
  if (it != this->header.end())
    return it->second;

  return "";
}

void HttpResponseHeader::remove(const std::string& key) {
  std::map<std::string, std::string>::iterator  t;
  std::string                                   lkey;

  lkey = util::toLowerStr(key);
  t = this->header.end();
  for (std::map<std::string, std::string>::iterator it = this->header.begin(); it != this->header.end(); ++it) {
    if (it->first == lkey) {
      t = it;
      break;
    }
  }

  if (t != this->header.end())
    this->header.erase(t);
}

std::string HttpResponseHeader::toStringForResponse() {
  std::string ret;

  for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it)
    ret += makeHeaderStr(it->first, it->second);

  return ret;
}

std::string HttpResponseHeader::makeHeaderStr(const std::string& key, const std::string& value) {
  return key + ": " + value + "\r\n";
}
