#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "./HttpStatus.hpp"
# include "./HttpRequest.hpp"
# include "../Util.hpp"

# include <map>
# include <string>

class HttpResponse {

  public:
    HttpResponse();
    ~HttpResponse();

    void        setStatusCode(const HttpStatus statusCode);
    void        setHeader(const std::string& field, const std::string& value);
    void        setBody(const std::string& body);
    std::string toString();

  private:
    static const std::string            version;

    HttpStatus                          statusCode;
    std::string                         statusText;
    std::map<std::string, std::string>  header;
    std::string                         body;

    std::string                         makeStatusLine() const;
    std::string                         makeHeaderField(const std::string& fieldName, const std::string& value) const;
};

// TODO: move
std::string getNowStr();

#endif
