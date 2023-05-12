#ifndef UTIL_HPP
# define UTIL_HPP

# include <cstdlib>
# include <fstream>
# include <sstream>
# include <vector>
# include <map>
# include <iostream>
# include <unistd.h>

const std::string CRLF = "\r\n";

namespace util {
  std::vector<std::string> split(const std::string& str, char delim);
  std::vector<std::string> split(std::string s, const std::string& delim);
  size_t find(const std::string& str, const std::string& target);
  std::string trimSpace(std::string s);
  std::string readFile(const std::string& fileName);
  std::string toLowerStr(const std::string& s);
  std::string toUpperStr(const std::string& s);
  std::string itoa(int n);
  int atoi(std::string s);
  const std::string getMimeType(const std::string& filename);
  void writeFile(const std::string& filename, const std::string& data);
  int ftFork(void);
  void ftPipe(int* fd);
  std::string readFd(int fd);
  void ftFree(char** data);
  void ftDup2(int oldFd, int newFd);
  void ftExecve(const std::string& file, char* const* argv, char* const* envp);
  void ftClose(int fd);

  std::pair<std::string, std::string> splitHeaderBody(const std::string& str, const std::string& delim);
  std::map<std::string, std::string> parseCGIHeader(const std::string& str);
  std::pair<std::string, std::string> splitField(const std::string& str);
  std::map<std::string, std::string> splitHeaderField(const std::string& str);

  class StringFoundException : public std::exception
  {
    public:
      const char* what(void) const throw();
  };

  class IOException : public std::exception
  {
    public:
      const char* what(void) const throw();
  };

  class SystemFunctionException : public std::exception
  {
    public:
      const char* what(void) const throw();
  };
}

#endif
