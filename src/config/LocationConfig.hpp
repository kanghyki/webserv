#ifndef LOCATION_CONFIG
# define LOCATION_CONFIG

# include "./CommonConfig.hpp"

# include <string>
# include <vector>
# include <map>

class ServerConfig;

class LocationConfig: public CommonConfig {
  public:
    LocationConfig();
    LocationConfig(const ServerConfig& config);
    ~LocationConfig();
    LocationConfig(const LocationConfig& obj);
    LocationConfig& operator=(const LocationConfig& obj);

    std::string                         getPath() const;
    std::string                         getAlias() const;
    std::string                         getLimitExcept() const;
    std::map<int, std::string>          getReturnRes() const;
    bool                                isAutoIndex() const;
    const std::vector<LocationConfig>&  getLocationConfig() const;

    void                                setPath(std::string path);
    void                                setAlias(std::string alias);
    void                                setLimitExcept(std::string limitExcept);
    void                                addReturnRes(std::pair<int, std::string> returnRes);
    void                                setAutoIndex(bool autoIndex);
    void                                addLocationConfig(LocationConfig location);

  private:
    static const bool                   DEFAULT_AUTOINDEX;

    std::string                         path;
    std::string                         alias;
    std::string                         limitExcept;
    std::map<int, std::string>          returnRes;
    bool                                autoIndex;
    std::vector<LocationConfig>         locations;
};

#endif
