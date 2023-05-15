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

    std::string                         getAlias() const;
    std::string                         getPath() const;
    const std::vector<std::string>&     getLimitExcept() const;
    bool                                isMethodAllowed(std::string method) const;
    bool                                isSetReturn() const;
    std::pair<int, std::string>         getReturnRes() const;
    bool                                isAutoindex() const;
    const std::vector<LocationConfig>&  getLocationConfig() const;

    void                                setAlias(std::string alias);
    void                                setPath(std::string path);
    void                                setLimitExcept(std::vector<std::string> methods);
    void                                setReturnRes(int status, std::string path);
    void                                setAutoindex(bool autoindex);
    void                                addLocationConfig(LocationConfig location);

    std::string                         toStringLimitExcept() const;

  private:
    static const bool                   DEFAULT_AUTOINDEX;
    static const std::string            DEFAULT_ALIAS;
    static const std::string            DEFAULT_PATH;

    std::string                         alias;
    std::string                         path;
    std::vector<std::string>            limitExcept;
    std::pair<int, std::string>         returnRes;
    bool                                autoindex;
    std::vector<LocationConfig>         locations;
};

#endif
