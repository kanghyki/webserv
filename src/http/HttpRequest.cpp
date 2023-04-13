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

/*
 * ---------------------- Non-Member Function ----------------------
 */
