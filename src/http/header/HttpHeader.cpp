#include "./HttpHeader.hpp"

HttpHeader::HttpHeader() {
}

HttpHeader::~HttpHeader() {
}

HttpHeader::HttpHeader(const HttpHeader& obj):
  header(obj.header) {
}

void HttpHeader::set(const std::string& key, const std::string& value) {
  std::string lkey;

  lkey = util::toLowerStr(key);
  this->header.insert(std::make_pair(lkey, value));
}

std::string HttpHeader::get(const std::string& key) const {
  std::map<std::string, std::string>::const_iterator it;

  it = this->header.find(key);
  if (it != this->header.end())
    return it->second;

  return "";
}

void HttpHeader::remove(const std::string& key) {
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

std::map<std::string, std::string> HttpHeader::copy() const {
  return this->header;
}
