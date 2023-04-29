#ifndef HTTP_DATA_FETCHER_HPP
# define HTTP_DATA_FETCHER_HPP

# include "./DirectoryList.hpp"
# include "./HttpRequest.hpp"
# include "../Util.hpp"
# include "../config/ServerConfig.hpp"

# include <dirent.h>
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

  private:
    const HttpRequest&  req;
    std::string         autoindex() const                 throw(HttpStatus);
};

#endif
