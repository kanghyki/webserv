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
    std::string                         getCGIPath() const;
    std::string                         getCGIScriptPath() const;
    std::map<int, std::string>          getReturnRes() const;
    bool                                isAutoIndex() const;
    bool                                isCgi() const;
    bool                                isExecutable() const;
    const std::vector<LocationConfig>&  getLocationConfig() const;

    void                                setPath(std::string path);
    void                                setAlias(std::string alias);
    void                                setLimitExcept(std::string limitExcept);
    void                                setCGIPath(std::string p);
    void                                setCGIScriptPath(std::string p);
    void                                addReturnRes(std::pair<int, std::string> returnRes);
    void                                setAutoIndex(bool autoIndex);
    void                                setCgi(bool cgi);
    void                                setExecutable(bool executable);
    void                                addLocationConfig(LocationConfig location);

  private:
    static const bool                   DEFAULT_AUTOINDEX;
    static const bool                   DEFAULT_CGI;
    

    std::string                         path;
    std::string                         alias;
    std::string                         limitExcept;
    std::string                         cgiScriptPath;
    std::string                         cgiPath;
    std::map<int, std::string>          returnRes;
    bool                                autoIndex;
    std::vector<LocationConfig>         locations;
    bool                                cgi;
    bool                                executable;
};

#endif
