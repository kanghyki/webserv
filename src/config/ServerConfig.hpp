#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include "CommonConfig.hpp"
#include "LocationConfig.hpp"
#include <string>
#include <vector>

class ServerConfig: public CommonConfig {
  public:
    ServerConfig();
    ~ServerConfig();
    ServerConfig(const ServerConfig &obj);
    ServerConfig &operator=(const ServerConfig &obj);

    int getPort() const;
    void setPort(int port);
    std::string getHost() const;
    void setHost(std::string host);
    std::string getServerName() const;
    void setServerName(std::string serverName);
    std::vector<LocationConfig> getLocationConfig() const;
    void addLocationConfig(LocationConfig location);

  private:
    int port;
    std::string host;
    std::string serverName;
    std::vector<LocationConfig> locations;
};

#endif
