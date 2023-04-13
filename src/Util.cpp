#include "Util.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

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

namespace util {
  std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> ret;

    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim))
      ret.push_back(token);

    return ret;
  }

  std::string getline(const std::string& str) {
    std::stringstream ss(str);
    std::string ret;

    std::getline(ss, ret);
    return ret;
  }

  size_t find(const std::string& str, const std::string& target) {
    size_t n = str.find(target);

    if (n == std::string::npos)
      throw std::runtime_error("Target not found");
    
    return n;
  }
}
