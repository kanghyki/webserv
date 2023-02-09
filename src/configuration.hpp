#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>
#include <map>
#include <set>
#include <vector>

typedef struct AConfiguration {
  int client_body_size;
  std::string root;
  std::map<int, std::string> error_page;
  std::set<std::string> index;
} AConfiguration;

typedef struct LocationConfiguration: AConfiguration {
  std::string path;
  std::string alias;
  std::string limit_except;
  std::map<int, std::string> return_res;
  bool auto_index;

  std::vector<LocationConfiguration *> locations;
} LocationConfiguration;

typedef struct ServerConfiguration: AConfiguration {
  int port;
  std::string host;
  std::string server_name;

  std::vector<LocationConfiguration *> locations;
} ServerConfiguration;

typedef struct HttpConfiguration: AConfiguration {
  std::vector<ServerConfiguration *> servers;
} HttpConfiguration;

typedef struct Configuration {
  HttpConfiguration *http;
} Configuration;

#endif
