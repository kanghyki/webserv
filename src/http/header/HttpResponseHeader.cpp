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
  this->header.set(key, value);
}

std::string HttpResponseHeader::get(const std::string& key) const {
  return this->header.get(key);
}

void HttpResponseHeader::remove(const std::string& key) {
  this->header.remove(key);
}

std::string HttpResponseHeader::toStringForResponse() {
  std::string                         ret;
  std::map<std::string, std::string>  hd;

  hd = this->header.getCopy();
  for (std::map<std::string, std::string>::iterator it = hd.begin(); it != hd.end(); ++it)
    ret += makeHeaderStr(it->first, it->second);

  return ret;
}

std::string HttpResponseHeader::makeHeaderStr(const std::string& key, const std::string& value) {
  return key + ": " + value + "\r\n";
}
