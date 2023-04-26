#ifndef URI_PARSER_HPP
# define URI_PARSER_HPP

# include "../Util.hpp"

# include <map>
# include <string>
# include <vector>

class UriParser {
  public:
    UriParser(const std::string& req);
    ~UriParser();

    std::string                         getPath();
    std::map<std::string, std::string>  getQuery();

  private:
    std::string                         path;
    std::map<std::string, std::string>  query;

    void                                parseQuery(const std::string& query);
};

#endif
