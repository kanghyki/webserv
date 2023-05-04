#ifndef HTTP_CONFIG_HPP
# define HTTP_CONFIG_HPP

# include "./CommonConfig.hpp"
# include "./ServerConfig.hpp"

# include <vector>

class HttpConfig: public CommonConfig {
  public:
    HttpConfig();
    ~HttpConfig();
    HttpConfig(const HttpConfig& obj);
    HttpConfig& operator=(const HttpConfig& obj);

    const ServerConfig&              findServerConfig(std::string reqHost) const;

    const std::vector<ServerConfig>& getServerConfig() const;
    void                             addServerConfig(ServerConfig server);



  private:
    std::vector<ServerConfig> servers;
};

#endif
