#ifndef UTIL_HPP
# define UTIL_HPP

# include <fstream>
# include <sstream>
# include <vector>
# include <map>

namespace util {
  std::vector<std::string> split(const std::string& str, char delim);
  std::vector<std::string> split(std::string s, const std::string& delim);
  size_t find(const std::string& str, const std::string& target);
  std::string trimSpace(std::string s);
  std::string readFile(const std::string& fileName);
  std::string toLowerStr(std::string s);
  std::string itoa(int n);
  const std::string getMimeType(const std::string& filename);
  void writeFile(const std::string& filename, const std::string& data);

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
}

#endif
