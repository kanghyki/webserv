#include "Http.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Http::Http(const std::string& header) : headerStr(header) {
  this->versionProtocol = parseVersion(header);
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

std::string parseVersion(const std::string& header) {
  std::string startLine = util::getline(header);
  return (util::split(startLine, ' ')[0]);
}

/*
 * ---------------------- Non-Member Function ----------------------
 */
