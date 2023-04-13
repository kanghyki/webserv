#ifndef UTIL_HPP
# define UTIL_HPP

# include <vector>
# include <sstream>

namespace util {
  std::vector<std::string> split(const std::string& str, char delim);
  std::string getline(const std::string& str);
  size_t find(const std::string& str, const std::string& target);
}


#endif
