#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "Http.hpp"
# include <stdexcept>


class HttpRequest : public Http {
  enum { methodSize = 3 };
  std::string methods[methodSize] = {"GET", "POST", "DELETE"};
  const std::string GET = "GET";
  const std::string POST = "POST";
  const std::string DELETE = "DELETE";
//  enum Method {
//    GET,
//    POST,
//    DELETE
//  };

  public:
    HttpRequest(const std::string& data);

  private:
    std::string method;
    std::string path;
    std::string version;

    void parseRequest(const std::string& startLine);
};

#endif
