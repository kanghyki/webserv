#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "Http.hpp"

class HttpResponse : public Http{
  enum Status {
    Continue = 100,
    OK = 200,
    BadRequest = 400,
    Forbidden = 403,
    NotFound = 404,
    InternalServerError = 500
  };

  public:

  private:
    int statusCode;
    std::string statusMessage;
};

#endif
