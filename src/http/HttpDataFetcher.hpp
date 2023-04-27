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
    std::string         fetch() const;
    static std::string  readFile(const std::string& path);
    const std::string   getData(void) const;
    const std::string   getMimeType(void) const;

  private:
    const HttpRequest&  request;

    std::string         excuteCGI(const std::string& path) const;
};

#endif
