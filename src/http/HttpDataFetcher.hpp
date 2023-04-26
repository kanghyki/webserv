#ifndef HTTP_DATA_FETCHER_HPP
# define HTTP_DATA_FETCHER_HPP

# include "./HttpRequest.hpp"
# include "../config/ServerConfig.hpp"
# include "../Util.hpp"

# include <string>
# include <string.h>
# include <dirent.h>
# include <iostream>
# include <utility>

class HttpDataFecther {
  public:
    HttpDataFecther(HttpRequest request, ServerConfig config);
    ~HttpDataFecther();
    std::string         fetch() const;
    static std::string  readFile(const std::string &path);
    const std::string   getData(void) const;
    const std::string   getMimeType(void) const;
    std::string         excuteCGI(const std::string &path) const;
    std::string         readDirectory() const;

  private:
    static const int READ  = 0;
    static const int WRITE = 1;

    HttpRequest         request;
    ServerConfig        config;

};

#endif
