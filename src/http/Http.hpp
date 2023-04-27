#ifndef HTTP_HPP
# define HTTP_HPP

# include "../config/ServerConfig.hpp"
# include "./HttpStatus.hpp"
# include "./HttpDataFetcher.hpp"
# include "./HttpRequest.hpp"
# include "./HttpResponse.hpp"
# include "./HttpResponseBuilder.hpp"

# include <vector>
# include <utility>
# include <dirent.h>

class Http {
  public:
    Http();
    ~Http();

    HttpResponse  processing(const HttpRequest& req);
    HttpResponse  getErrorPage(HttpStatus s, const ServerConfig& config);

  private:
    HttpResponse  getMethod(const HttpRequest& req);
    HttpResponse  postMethod(const HttpRequest& req);
    HttpResponse  deleteMethod(const HttpRequest& req);
    HttpResponse  putMethod(const HttpRequest& req);
};

#endif
