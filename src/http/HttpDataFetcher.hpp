#ifndef HTTP_DATA_FETCHER_HPP
# define HTTP_DATA_FETCHER_HPP

# include "./HttpRequest.hpp"
# include "../etc/Util.hpp"
# include "../config/ServerConfig.hpp"

# include <string>
# include <iostream>
# include <utility>
# include <dirent.h>
# include <string.h>
# include <sys/stat.h>

class HttpDataFecther {
  public:
    HttpDataFecther(const HttpRequest& request);
    ~HttpDataFecther();
    std::string         fetch() const;
    static std::string  readFile(const std::string& path);

  private:
    const HttpRequest&  req;
    std::string         autoindex() const;
};

#endif
