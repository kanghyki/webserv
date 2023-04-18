#include "Http.hpp"
#include <utility>

/*
 * -------------------------- Constructor --------------------------
 */

Http::Http(const std::string& data) {
  this->header = parseHeader(getHeader(data));
  this->body = getBody(data);
}

/*
 * -------------------------- Destructor ---------------------------
 */

/*
 * -------------------------- Operator -----------------------------
 */

/*
 * -------------------------- Getter -------------------------------
 */

/*
 * -------------------------- Setter -------------------------------
 */

/*
 * ----------------------- Member Function -------------------------
 */

std::map<std::string, std::string> Http::parseHeader(const std::string& header) {
  std::map<std::string, std::string> ret;
  std::stringstream ss(header);
  std::string line;
  
  while (std::getline(ss, line)) {
    size_t pos = line.find(':');
    std::string key = util::trimSpace(line.substr(0, pos));
    std::string value = util::trimSpace(line.substr(pos + 1));
    ret.insert(std::make_pair(key, value));
  }

  return ret;
}

/*
 * ---------------------- Non-Member Function ----------------------
 */

std::string getStartLine(const std::string& data) {
  try {
    size_t n = util::find(data, "\r\n");
    return data.substr(0, n);
  } catch (std::exception& e) {
    throw except::HttpRequestException();
  }
}

std::string getHeaderBody(const std::string& data) {
  try {
    size_t n = util::find(data, "\r\n");
    return data.substr(n);
  } catch (std::exception& e) {
    throw except::HttpRequestException();
  }
}

std::string getHeader(const std::string& data) {
  try {
    size_t n = util::find(data, "\r\r\n");
    return data.substr(0, n);
  } catch (std::exception& e) {
    throw except::HttpRequestException();
  }
}

std::string getBody(const std::string& data) {
  try {
    size_t n = util::find(data, "\r\r\n");
    return data.substr(n);
  } catch (std::exception& e) {
    return "";
  }
}
