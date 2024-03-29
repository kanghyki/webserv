#ifndef HTTP_HPP
# define HTTP_HPP

# include "./HttpStatus.hpp"
# include "./HttpDataFetcher.hpp"
# include "./HttpRequest.hpp"
# include "./HttpResponse.hpp"
# include "./CGI.hpp"
# include "../config/ServerConfig.hpp"
# include "../etc/Logger.hpp"
# include "../network/SessionManager.hpp"

# include <vector>
# include <utility>
# include <dirent.h>

class Http {
  public:
    Http();
    ~Http();

    static HttpResponse processing(const HttpRequest& req, SessionManager& manager);
    static HttpResponse getErrorPage(HttpStatus s, const HttpRequest& req);
    static void         finishCGI(HttpResponse& res, const HttpRequest& req, SessionManager& sm);

  private:
    static void         checkAndThrowError(const HttpRequest& req);
    static HttpResponse executeCGI(const HttpRequest& req, SessionManager& sm);
    static HttpResponse getMethod(const HttpRequest& req);
    static HttpResponse postMethod(const HttpRequest& req);
    static HttpResponse deleteMethod(const HttpRequest& req);
    static HttpResponse putMethod(const HttpRequest& req);

    static std::string  generateDefaultErrorPage(HttpStatus s);
    static std::string  generateAutoindex(const HttpRequest& req);
};

#endif
