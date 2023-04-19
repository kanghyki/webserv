#ifndef UTIL_HPP
# define UTIL_HPP

# include <vector>
# include <sstream>
# include <fstream>

namespace util {
  std::vector<std::string> split(const std::string& str, char delim);
  std::vector<std::string> split(const std::string& str, const std::string& delim);
  size_t find(const std::string& str, const std::string& target);
  std::string trimSpace(std::string s);
  std::string readFile(const std::string& fileName);

  class StringFoundException : public std::exception
  {
    public:
      const char* what(void) const throw();
  };

  class FileOpenException : public std::exception
  {
    public:
      const char* what(void) const throw();
  };
}

#endif
