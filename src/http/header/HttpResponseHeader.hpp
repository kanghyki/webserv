#ifndef HTTP_RESPONSE_HEADER_HPP
# define HTTP_RESPONSE_HEADER_HPP

# include "./HttpHeader.hpp"
# include "../../etc/Util.hpp"

# include <string>
# include <map>

class HttpResponseHeader {
  public:
    static const std::string CONTENT_TYPE;
    static const std::string CONTENT_LENGTH;
    static const std::string DATE;
    static const std::string KEEP_ALIVE;
    static const std::string LOCATION;
    static const std::string ALLOW;
    static const std::string SERVER;
    static const std::string CONNECTION;
    static const std::string UPGRADE;
    static const std::string SET_COOKIE;

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

    HttpHeader  header;

    std::string makeHeaderStr(const std::string& key, const std::string& value);
};

#endif
