#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "./HttpStatus.hpp"

# include <map>
# include <string>

class HttpResponse {

  public:
    HttpResponse();
    ~HttpResponse();

    void        setStatusCode(const HttpStatus statusCode);
    void        setHttpVersion(const std::string& version);
    void        setHeader(const std::string& field, const std::string& value);
    void        setBody(const std::string& body);
    std::string toString();

  private:
    HttpStatus                          statusCode;
    std::string                         statusText;
    std::string                         version;
    std::map<std::string, std::string>  header;
    std::string                         body;
};

// TODO: move
std::string getNowStr();

#endif
