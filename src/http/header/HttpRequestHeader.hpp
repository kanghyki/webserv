#ifndef HTTP_REQUEST_HEADER_HPP
# define HTTP_REQUEST_HEADER_HPP

# include "../../etc/Util.hpp"
# include "../HttpStatus.hpp"

# include <string>
# include <map>

class HttpRequestHeader {
  public:
    static const std::string  HOST;
    static const std::string  TRANSFER_ENCODING;
    static const std::string  CONNECTION;
    static const std::string  CONTENT_TYPE;
    static const std::string  CONTENT_LENGTH;
    static const std::string  COOKIE;

    HttpRequestHeader();
    ~HttpRequestHeader();
    HttpRequestHeader& operator=(const HttpRequestHeader& obj);

    enum connection {
      KEEP_ALIVE,
      CLOSE
    };

    enum transfer_encoding {
      UNSET,
      CHUNKED
    };

    void parse(const std::string& reqHeader);

    std::string                               get(std::string key) const;
    const std::map<std::string, std::string>& getOrigin() const;
    connection                                getConnection() const;
    transfer_encoding                         getTransferEncoding() const;
    const std::map<std::string, std::string>  getCustomeHeader() const;

    void                                      setConnection(connection conn);

  private:
    std::map<std::string, std::string>  header;
    connection                          conn;
    transfer_encoding                   te;

    void parseConnection();
    void parseTransferEncoding();

    void setTransferEncoding(transfer_encoding te);

    std::pair<std::string, std::string> splitField(std::string line);
};

#endif
