#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>

class HttpResponse {
  enum Status {
    Continue = 100,
    OK = 200,
    BadRequest = 400,
    Forbidden = 403,
    NotFound = 404,
    InternalServerError = 500
  };

  public:
    HttpResponse(const std::string& data);

  private:
    int statusCode;
    std::string statusMessage;
    std::string version;

    void parseResponse(const std::string& startLine);
};

#endif
