#ifndef HTTP_HPP
#define HTTP_HPP

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include "util.hpp"

class Http {
  public:
    Http(std::string header_str);
    std::map<std::string, std::string> getHeader(void);
    std::string getHeaderStr(void);

  private:
    std::map<std::string, std::string> header;
    std::string header_str;
    void ConvertHeader(std::string header_str);
    void ConvertHeader(std::map<std::string, std::string>);
};

#endif
