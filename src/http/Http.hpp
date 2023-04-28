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

    static HttpResponse processing(const HttpRequest req) throw (HttpStatus);
    static HttpResponse getErrorPage(HttpStatus s, const LocationConfig& config);

  private:
    static HttpResponse getMethod(const HttpRequest& req);
    static HttpResponse postMethod(const HttpRequest& req);
    static HttpResponse deleteMethod(const HttpRequest& req);
    static HttpResponse putMethod(const HttpRequest& req);
};

#endif
