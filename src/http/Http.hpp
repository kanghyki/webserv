#ifndef HTTP_HPP
# define HTTP_HPP

# include <iostream>
# include <map>
# include "../Util.hpp"

class Http {
  protected:
    Http(const std::string& data);

  private:
    std::map<std::string, std::string> header;
    std::string body;

    std::map<std::string, std::string> parseHeader(const std::string& header);
};

std::string getStartLine(const std::string& data);
std::string getHeaderBody(const std::string& data);
std::string getHeader(const std::string& data);
std::string getBody(const std::string& data);

#endif
