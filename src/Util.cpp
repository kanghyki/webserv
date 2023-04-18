#include "Util.hpp"
#include "Except.hpp"

namespace util {
  std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> ret;

    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim))
      ret.push_back(token);

    return ret;
  }

  std::vector<std::string> split(const std::string& str, const std::string& delim) {
    std::vector<std::string> ret;
    size_t start = 0;
    size_t pos = 0;

    while ((pos = str.find(delim, start)) != std::string::npos) {
      ret.push_back(str.substr(start, pos));
      start += pos + delim.length();
    }
    ret.push_back(str.substr(start));

    return ret;
  }

  size_t find(const std::string& str, const std::string& target) {
    size_t n = str.find(target);

    if (n == std::string::npos)
      throw except::StringFoundException();
    
    return n;
  }

  std::string trimSpace(std::string s) {
    size_t f, r;

    f = 0;
    while (f < s.length() && std::isspace(s[f]))
      ++f;
    s.erase(0, f);
    r = s.length() - 1;
    while (r > 0 && std::isspace(s[r]))
      --r;
    s.erase(r + 1, s.length() - 1 - r);

    return s;
  }

  std::string readFile(const std::string& fileName) {
    std::ifstream in(fileName, std::ifstream::in);
    std::string line;
    std::string ret;

    if (!in.is_open())
      throw except::FileOpenException();
    while (std::getline(in, line))
      ret += line;

    return ret;
  }
}
