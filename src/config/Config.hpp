#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "HttpConfig.hpp"
#include <vector>

class Config {
  public:
    Config();
    ~Config();
    Config(const Config &obj);
    Config &operator=(const Config &obj);

    std::vector<HttpConfig> getHttpConfig() const;
    void addHttpConfig(HttpConfig http);

    Config &parseServerConfig();

  private:
    std::vector<HttpConfig> https;
};

#endif
