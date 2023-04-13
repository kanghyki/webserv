#include "Http.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Http::Http(const std::string& data) {
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

std::string getStartLine(const std::string& data) {
  try {
    size_t n = util::find(data, "\r\n");
    return data.substr(0, n);
  } catch (std::exception& e) {
    throw std::runtime_error("Data not in compliance with HTTP protocol");
  }
}

std::string getHeaderBody(const std::string& data) {
  try {
    size_t n = util::find(data, "\r\n");
    return data.substr(n);
  } catch (std::exception& e) {
    throw std::runtime_error("Data not in compliance with HTTP protocol");
  }
}

std::string getHeader(const std::string& data) {
  try {
    size_t n = util::find(data, "\r\r\n");
    return data.substr(0, n);
  } catch (std::exception& e) {
    throw std::runtime_error("Data not in compliance with HTTP protocol");
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

/*
 * ---------------------- Non-Member Function ----------------------
 */
