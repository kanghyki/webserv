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
    ~ServerConfig();
    ServerConfig(const ServerConfig& obj);
    ServerConfig& operator=(const ServerConfig& obj);

    const LocationConfig        findLocationConfig(const std::string& path) const;

    short                       getPort() const;
    std::string                 getHost() const;
    std::string                 getServerName() const;
    std::vector<LocationConfig> getLocationConfig() const;

    void                        setPort(short port);
    void                        setHost(std::string host);
    void                        setServerName(std::string serverName);
    void                        addLocationConfig(LocationConfig location);

  private:
    static const short          DEFAULT_PORT;
    static const std::string    DEFAULT_HOST;

    short                       port;
    std::string                 host;
    std::string                 serverName;
    std::vector<LocationConfig> locations;
};

#endif
