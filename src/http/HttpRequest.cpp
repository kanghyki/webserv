#include "HttpRequest.hpp"
#include "Http.hpp"

const std::string HttpRequest::GET = "GET";
const std::string HttpRequest::POST = "POSt";
const std::string HttpRequest::DELETE = "DELETE";

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
    throw except::HttpRequestException();
  // TODO: validate
  this->method = ss[0]; 
  // TODO: validate
  this->path = ss[1];
  // TODO: validate
  this->version = ss[2];
}


/*
 * ---------------------- Non-Member Function ----------------------
 */
