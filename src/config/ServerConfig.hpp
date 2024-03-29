#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

# include "./CommonConfig.hpp"
# include "./LocationConfig.hpp"

# include <string>
# include <vector>

class HttpConfig;

class ServerConfig: public CommonConfig {
  public:
    ServerConfig();
    ServerConfig(const HttpConfig& config);
    ServerConfig(const ServerConfig& obj);
    ~ServerConfig();
    ServerConfig& operator=(const ServerConfig& obj);

    const LocationConfig                      findLocationConfig(std::string path) const;

    int                                       getGatewayTimeout() const;
    int                                       getSessionTimeout() const;
    int                                       getKeepAliveTimeout() const;
    int                                       getKeepAliveRequests() const;
    short                                     getPort() const;
    std::string                               getHost() const;
    std::string                               getServerName() const;
    const std::map<std::string, std::string>& getCGI() const;
    const std::vector<LocationConfig>&        getLocationConfig() const;

    void                                      setGatewayTimeout(int n);
    void                                      setSessionTimeout(int n);
    void                                      setKeepAliveTimeout(int n);
    void                                      setKeepAliveRequests(int n);
    void                                      setPort(short port);
    void                                      setHost(std::string host);
    void                                      setServerName(std::string serverName);
    void                                      insertCGI(std::string ext, std::string path);
    void                                      addLocationConfig(LocationConfig location);

  private:
    static const int                          DEFAULT_GATEWAY_TIMEOUT;
    static const int                          DEFAULT_SESSION_TIMEOUT;
    static const int                          DEFAULT_KEEPALIVE_TIMEOUT;
    static const int                          DEFAULT_KEEPALIVE_REQUESTS;
    static const short                        DEFAULT_PORT;
    static const std::string                  DEFAULT_HOST;

    int                                       gateway_timeout;
    int                                       session_timeout;
    int                                       keepalive_timeout;
    int                                       keepalive_requests;
    short                                     port;
    std::string                               host;
    std::string                               serverName;
    std::map<std::string, std::string>        cgi;
    std::vector<LocationConfig>               locations;

    const LocationConfig&                     findLocationConfigRoop(const LocationConfig& config, std::string path) const;
};

#endif
