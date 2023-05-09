#ifndef HTTP_CONFIG_HPP
# define HTTP_CONFIG_HPP

# include "./CommonConfig.hpp"
# include "./ServerConfig.hpp"

# include <stdexcept>
# include <vector>

class HttpConfig: public CommonConfig {
  public:
    HttpConfig();
    ~HttpConfig();
    HttpConfig(const HttpConfig& obj);
    HttpConfig& operator=(const HttpConfig& obj);

    const ServerConfig&               findServerConfig(std::string reqHost) const;

    int                               getClientHeaderTimeout() const;
    int                               getClientBodyTimeout() const;
    int                               getSendTimeout() const;
    const std::vector<ServerConfig>&  getServerConfig() const;

    void                              setClientHeaderTimeout(int n);
    void                              setClientBodyTimeout(int n);
    void                              setSendTimeout(int n);
    void                              addServerConfig(ServerConfig server);

  private:
    static const int                  DEFAULT_CLIENT_HEADER_TIMEOUT;
    static const int                  DEFAULT_CLIENT_BODY_TIMEOUT;
    static const int                  DEFAULT_SEND_TIMEOUT;

    int                               client_header_timeout;
    int                               client_body_timeout;
    int                               send_timeout;
    std::vector<ServerConfig>         servers;
};

#endif
