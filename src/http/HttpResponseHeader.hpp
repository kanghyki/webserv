#ifndef HTTP_RESPONSE_HEADER_HPP
# define HTTP_RESPONSE_HEADER_HPP

# include "../etc/Util.hpp"

# include <string>
# include <map>

class HttpResponseHeader {
  public:
    HttpResponseHeader();
    HttpResponseHeader(const HttpResponseHeader& obj);
    ~HttpResponseHeader();
    HttpResponseHeader& operator=(const HttpResponseHeader& obj);

    void        set(const std::string& key, const std::string& value);
    std::string get(const std::string& key) const;

    // Delete all matching headers, case-insensitive
    void        remove(const std::string& key);
    std::string toStringForResponse();

  private:

    std::map<std::string, std::string>  header;

    std::string makeHeaderStr(const std::string& key, const std::string& value);
};

#endif
