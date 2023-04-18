#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "Http.hpp"

class HttpRequest : public Http {
  static const std::string GET;
  static const std::string POST;
  static const std::string DELETE;
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
