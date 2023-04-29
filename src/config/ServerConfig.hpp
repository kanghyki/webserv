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
    ~ServerConfig();
    ServerConfig(const ServerConfig& obj);
    ServerConfig& operator=(const ServerConfig& obj);

    const LocationConfig                findLocationConfig(std::string path) const;
    const LocationConfig&               findLocationConfigRoop(const LocationConfig& config, std::string path) const;

    int                                       getTimeout() const;
    short                                     getPort() const;
    std::string                               getHost() const;
    std::string                               getServerName() const;
    const std::map<std::string, std::string>& getCGI() const;
    const std::vector<LocationConfig>&        getLocationConfig() const;

    void                                      setTimeout(int n);
    void                                      setPort(short port);
    void                                      setHost(std::string host);
    void                                      setServerName(std::string serverName);
    void                                      insertCGI(std::string ext, std::string path);
    void                                      addLocationConfig(LocationConfig location);

  private:
    static const int                          DEFAULT_TIMEOUT;
    static const short                        DEFAULT_PORT;
    static const std::string                  DEFAULT_HOST;

    int                                       timeout;
    short                                     port;
    std::string                               host;
    std::string                               serverName;
    std::map<std::string, std::string>        cgi;
    std::vector<LocationConfig>               locations;
};

#endif
