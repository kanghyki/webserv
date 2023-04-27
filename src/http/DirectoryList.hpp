#ifndef DIRECTORY_LIST_HPP
# define DIRECTORY_LIST_HPP

# include "./HttpRequest.hpp"
# include "./HttpStatus.hpp"
# include "../Util.hpp"

# include <dirent.h>
# include <exception>
# include <string>

class DirectoryList {
  public:
    static std::string generate(const HttpRequest& req) throw(HttpStatus);
  private:
};

#endif
