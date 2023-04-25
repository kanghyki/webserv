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

    HttpResponse  processing(std::string request);
    HttpResponse  getErrorPage(HttpStatus status);

  private:
    ServerConfig  config;

    HttpResponse  getMethod(HttpRequest& req);
    HttpResponse  postMethod(HttpRequest& req);
    HttpResponse  deleteMethod(HttpRequest& req);
    HttpResponse  putMethod(HttpRequest& req);
};

#endif
