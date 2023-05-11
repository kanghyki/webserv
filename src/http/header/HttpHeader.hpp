#ifndef HTTP_HEADER_HPP
# define HTTP_HEADER_HPP

# include "../../etc/Util.hpp"

# include <string>
# include <map>

class HttpRequestHeader;
class HttpResponseHeader;

class HttpHeader {
  friend class HttpRequestHeader;
  friend class HttpResponseHeader;

  public:
    HttpHeader();
    ~HttpHeader();
    HttpHeader(const HttpHeader& obj);

  private:
    std::string                         get(const std::string& key) const;
    void                                set(const std::string& key, const std::string& value);
    void                                remove(const std::string& key);
    std::map<std::string, std::string>  getCopy() const;

    std::map<std::string, std::string>  header;
};

#endif
