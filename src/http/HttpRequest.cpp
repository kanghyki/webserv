#include "HttpRequest.hpp"
#include "Http.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

HttpRequest::HttpRequest(const std::string& data) : Http(getHeaderBody(data)) {
  parseRequest(getStartLine(data));
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

void HttpRequest::parseRequest(const std::string& startLine) {
  std::vector<std::string> ss = util::split(startLine, " ");

  if (ss.size() != 3)
    throw std::runtime_error("Data not in compliance with HTTP protocol");
  // TODO: validate
  this->method = ss[0]; 
  // TODO: validate
  this->path = ss[1];
  // TODO: validate
  this->version = ss[2];
}

void HttpRequest::parse


/*
 * ---------------------- Non-Member Function ----------------------
 */
