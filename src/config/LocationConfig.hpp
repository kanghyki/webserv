#ifndef LOCATION_CONFIG
#define LOCATION_CONFIG

#include "CommonConfig.hpp"
#include <string>
#include <vector>
#include <map>

class LocationConfig: public CommonConfig {
  public:
    LocationConfig();
    ~LocationConfig();
    LocationConfig(const LocationConfig &obj);
    LocationConfig &operator=(const LocationConfig &obj);

    std::string getPath() const;
    void setPath(std::string path);
    std::string getAlias() const;
    void setAlias(std::string alias);
    std::string getLimitExcept() const;
    void setLimitExcept(std::string limitExcept);
    std::map<int, std::string> getReturnRes() const;
    void addReturnRes(std::pair<int, std::string> returnRes);
    bool isAutoIndex() const;
    void setAutoIndex(bool autoIndex);
    std::vector<LocationConfig> getLocationConfig() const;
    void addLocationConfig(LocationConfig location);

  private:
    std::string path;
    std::string alias;
    std::string limitExcept;
    std::map<int, std::string> returnRes;
    bool autoIndex;
    std::vector<LocationConfig> locations;
};

#endif
