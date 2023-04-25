#ifndef HTTP_RESPONSE_BUILDER_HPP
# define HTTP_RESPONSE_BUILDER_HPP

# include "./HttpResponse.hpp"
# include "./HttpStatus.hpp"

# include <string>

class HttpResponseBuilder {
  public:
    HttpResponseBuilder();
    ~HttpResponseBuilder();

    static HttpResponseBuilder  getBuilder();
    HttpResponseBuilder&        statusCode(const HttpStatus code);

    HttpResponseBuilder&        header(const std::string &key, const std::string &value);
    HttpResponseBuilder&        body(std::string body);
    HttpResponse                build();


  private:
    HttpResponse response;
};

#endif
