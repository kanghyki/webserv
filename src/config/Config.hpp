#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "ServerConfig.hpp"

# include <vector>

class Config {
  public:
    Config();
    ~Config();
    Config(const Config& obj);

    const std::vector<ServerConfig>&  getServerConfig() const;
    void                              addServerConfig(ServerConfig server);

    const ServerConfig&               findServerConfig(std::string reqHost) const;

  private:
    std::vector<ServerConfig> servers;
};

#endif
