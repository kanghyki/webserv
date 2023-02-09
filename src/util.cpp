#include "util.hpp"

namespace util {
  std::vector<std::string> Split(std::string str, char delim) {
    std::vector<std::string> ret;

    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim))
      ret.push_back(token);

    return ret;
  }
}

