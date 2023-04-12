#ifndef UTIL_HPP
# define UTIL_HPP

# include <vector>
# include <sstream>

namespace util {
  std::vector<std::string> split(const std::string& str, char delim);
  std::string getline(const std::string& str);
}


#endif
