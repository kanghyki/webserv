#ifndef HTTP_DATA_FETCHER_HPP
# define HTTP_DATA_FETCHER_HPP

# include "./HttpRequest.hpp"
# include "../config/ServerConfig.hpp"
# include "../Util.hpp"

# include <string>
# include <string.h>
# include <iostream>
# include <utility>

class HttpDataFecther {
  public:
    HttpDataFecther(const HttpRequest& request);
    ~HttpDataFecther();

    std::string         fetch() const                     throw(HttpStatus);
    static std::string  readFile(const std::string& path) throw(HttpStatus);
    const std::string   getData(void) const               throw(HttpStatus);
    const std::string   getMimeType(void) const           throw(HttpStatus);

  private:
    const HttpRequest&  req;

    std::string         excuteCGI(const std::string& path) const;
};

#endif
