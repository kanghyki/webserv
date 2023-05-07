#include "Util.hpp"

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
    if (s.empty() == false)
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
      ret += line + "\n";

    in.close();
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

  void  writeFile(const std::string& filename, const std::string& data) {
    std::ofstream out(filename, std::ofstream::out);
    if (!out.is_open()) throw util::IOException();
    
    out.write(data.c_str(), data.length());
    if (out.fail() || out.bad() || out.eof()) throw util::IOException();
  }

  int ftFork(void) {
    int pid = fork();

    if (pid == -1) throw util::SystemFunctionException();

    return pid;
  }

  std::string readFd(int fd) {
    std::string ret;
    int readSize;
    int bufSize = 128;
    char buf[bufSize + 1];

    while ((readSize = read(fd, buf, bufSize)) > 0) {
      buf[readSize] = 0;
      ret += buf;
    }

    return ret;
  }

  void ftPipe(int* fd) {
    if (pipe(fd) == -1)  throw util::SystemFunctionException();
  }

  void ftFree(char** data) {
    int i = 0;

    if (!data)
      return ;
    while (data[i]) {
      free(data[i]);
      i++;
    }
    free(data);
  }

  std::pair<std::string, std::string> splitHeaderBody(const std::string& str, const std::string& delim) {
    size_t pos = str.find(delim);

    std::pair<std::string, std::string> ret(str.substr(0, pos), str.substr(pos + 1));
    return ret;
  }

  std::map<std::string, std::string> parseCGIHeader(const std::string& str) {
    std::map<std::string, std::string> ret;
    std::vector<std::string>            vs;
    std::vector<std::string>::iterator  it;

    vs = util::split(str, CRLF);
    it = vs.begin();
    while (it != vs.end()) {
      std::pair<std::string, std::string> p = util::splitField(*it);
      util::splitField(*it);
      ret.insert(p);
      ++it;
    }

    return ret;
  }

  std::pair<std::string, std::string> splitField(const std::string& line) {
    std::string field = "";
    std::string value = "";
    int pos;

    if ((pos = line.find(":")) != -1) {
      field = util::toLowerStr(util::trimSpace(line.substr(0, pos)));
      value = util::trimSpace(line.substr(pos + 1));
    }

    return std::make_pair(field, value);
  }

  std::map<std::string, std::string> splitHeaderField(const std::string& str) {
    std::map<std::string, std::string> ret;

    std::vector<std::string> tmp = util::split(str, "; ");
    for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end(); ++it) {
      std::vector<std::string> v = util::split(*it, "=");
      ret.insert(std::pair<std::string, std::string>(v[0], v[1]));
    }

    return ret;
  }

  const char* StringFoundException::what() const throw() {
    return "Target not found";
  }

  const char* IOException::what() const throw() {
    return "File open failed";
  }

  const char* SystemFunctionException::what() const throw() {
    return "System fuction failed";
  }
}
