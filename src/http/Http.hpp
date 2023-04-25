#ifndef HTTP_HPP
# define HTTP_HPP

# include "../config/ServerConfig.hpp"
# include "./HttpDataFetcher.hpp"
# include "./HttpRequest.hpp"
# include "./HttpResponse.hpp"
# include "./HttpResponseBuilder.hpp"

# include <vector>
# include <utility>

class Http {
  public:
    Http(ServerConfig config);
    ~Http();

    HttpResponseBuilder processing(std::string request);
    HttpResponseBuilder getErrorPage(HttpStatus status);

  private:
    ServerConfig        config;

    HttpResponseBuilder getMethod(HttpRequest& req);
    HttpResponseBuilder postMethod(HttpRequest& req);
    HttpResponseBuilder deleteMethod(HttpRequest& req);
    HttpResponseBuilder putMethod(HttpRequest& req);
};

#endif
