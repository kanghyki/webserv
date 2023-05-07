#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "HttpConfig.hpp"

class Config {
  public:
    Config();
    ~Config();
    Config(const Config& obj);
    Config&     operator=(const Config& obj);

    HttpConfig  getHttpConfig() const;
    void        setHttpConfig(HttpConfig http);

  private:
    HttpConfig  http;
    bool        is_already_set;
};

#endif
