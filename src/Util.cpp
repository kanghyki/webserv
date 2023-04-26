#include "Util.hpp"

#include <iostream>
namespace util {
  std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> ret;

    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim))
      ret.push_back(token);

    return ret;
  }


  std::vector<std::string> split(std::string s, const std::string& delim) {
    std::vector<std::string> ret;
    unsigned long pos = 0;
    std::string token = "";

    while ((pos = s.find(delim)) != std::string::npos) {
      token = s.substr(0, pos);
      ret.push_back(token);
      s.erase(0, pos + delim.length());
    }
    ret.push_back(s);
    return ret;
  }

  size_t find(const std::string& str, const std::string& target) {
    size_t n = str.find(target);

    if (n == std::string::npos)
      throw util::StringFoundException();
    
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

    if (!in.is_open()) throw util::IOException();
    while (std::getline(in, line))
      ret += line;

    return ret;
  }

  std::string toLowerStr(std::string s) {
    for (size_t i = 0; i < s.length(); ++i)
      if (std::isupper(s[i]))
        s[i] = std::tolower(s[i]);
    return s;
  }

  std::string itoa(int n) {
    std::stringstream ss;

    ss << n;

    return ss.str();
  }

  const std::string getMimeType(const std::string& filename) {
    int BUF_SIZE = 128;
    std::string ret;
    char buf[BUF_SIZE + 1];
    std::string cmd = "file --brief --mime-type " + filename;
    FILE*       pipe = popen(cmd.c_str(), "r");
    int readSize;

    if (!pipe) throw util::IOException();

    while (!feof(pipe)) {
      readSize = fread(buf, 1, BUF_SIZE, pipe);
      buf[readSize] = 0;
      if (readSize != 0)
        ret += buf;
    }

    pclose(pipe);
    ret.erase(ret.find_last_not_of("\n\r") + 1);
 
    return ret;
  }

  void  writeFile(const std::string& filename, const std::string& data) {
    std::ofstream out(filename, std::ofstream::out);
    if (!out.is_open()) throw util::IOException();
    
    out.write(data.c_str(), data.length());
    if (out.fail() || out.bad() || out.eof()) throw util::IOException();
  }

  const char* StringFoundException::what() const throw() {
    return "Target not found";
  }

  const char* IOException::what() const throw() {
    return "File open failed";
  }
}
