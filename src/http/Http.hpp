#ifndef HTTP_HPP
# define HTTP_HPP

# include <iostream>
# include <map>
# include "../Util.hpp"

class Http {
  protected:
    Http(const std::string& header);

  private:
    std::map<std::string, std::string> headerMap;
    std::string headerStr;
    std::string body;
    std::string versionProtocol;

    std::map<std::string, std::string> parseHeader(const std::string& header);
    const std::string parseHeader(std::map<std::string, std::string> header);
    std::string parseVersion(const std::string& header);
};

#endif
