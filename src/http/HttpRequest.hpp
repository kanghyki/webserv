#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "Http.hpp"


class HttpRequest : public Http {
  enum Method {
    GET,
    POST,
    DELETE
  };

  public:
    HttpRequest(const std::string& data);

  private:
    int method;
    std::string path;
    std::string version;

    void parseRequest(const std::string& startLine);
};

#endif
