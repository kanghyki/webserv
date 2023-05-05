#ifndef LOCATION_CONFIG
# define LOCATION_CONFIG

# include "./CommonConfig.hpp"

# include <string>
# include <vector>
# include <utility>

class ServerConfig;

class LocationConfig: public CommonConfig {
  public:
    LocationConfig();
    LocationConfig(const ServerConfig& config);
    ~LocationConfig();
    LocationConfig(const LocationConfig& obj);
    LocationConfig& operator=(const LocationConfig& obj);

    std::string                         getPath() const;
    const std::vector<std::string>&     getLimitExcept() const;
    bool                                isMethodAllowed(std::string method) const;
    std::pair<int, std::string>         getReturnRes() const;
    bool                                isAutoindex() const;
    const std::vector<LocationConfig>&  getLocationConfig() const;

    void                                setPath(std::string path);
    void                                addLimitExcept(std::string method);
    void                                setReturnRes(int status, std::string path);
    void                                setAutoindex(bool autoindex);
    void                                addLocationConfig(LocationConfig location);

  private:
    static const bool                   DEFAULT_AUTOINDEX;
    static const std::string            DEFAULT_PATH;

    std::string                         path;
    std::vector<std::string>            limitExcept;
    std::pair<int, std::string>         returnRes;
    bool                                autoindex;
    std::vector<LocationConfig>         locations;
};

#endif
