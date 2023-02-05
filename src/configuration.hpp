#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>
#include <map>
#include <vector>

class CommonConfiguration {
  public:
    int client_body_size;
    std::string root;
    std::map<int, std::string> error_page;
    std::vector<std::string> index;
  private:
};

class LocationConfiguration {
  public:
    CommonConfiguration common;

    std::string alias;
    std::string limit_except;
    std::map<int, std::string> return_res;
    bool auto_index;

    std::vector<LocationConfiguration> locations;
  private:
};

class ServerConfiguration {
  public:
    CommonConfiguration common;

    int port;
    std::string host;
    std::string server_name;

    std::vector<LocationConfiguration> locations;
  private:
};

class HttpConfiguration {
  public:
    CommonConfiguration common;

    std::vector<ServerConfiguration> servers;
  private:
};

class Configuration {
  public:
  private:
    struct HttpConfiguration http;
};

#endif
