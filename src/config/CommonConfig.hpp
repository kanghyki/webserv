#ifndef COMMON_CONFIG_HPP
# define COMMON_CONFIG_HPP

# include <string>
# include <map>
# include <vector>

class CommonConfig {
  public:
    CommonConfig();
    CommonConfig(const CommonConfig& obj);
    virtual ~CommonConfig();
    CommonConfig&               operator=(const CommonConfig& obj);

    int                         getClientBodySize() const;
    std::string                 getRoot() const;
    std::string                 getErrorPagePath() const;
    std::vector<int>            getErrorPageStatus() const;
    std::vector<std::string>    getIndex() const;

    void                        setClientBodySize(int clientBodySize);
    void                        setRoot(std::string root);
    void                        setErrorPagePath(std::string path);
    void                        addErrorPageStatus(int n);
    void                        addIndex(std::string index);

  protected:
    int                         clientBodySize;
    std::string                 root;
    std::vector<std::string>    index;
    std::string                 errorPagePath;
    std::vector<int>            errorPageStatus;

  private:
    static const int            DEFAULT_CLIENT_BODY_SIZE;
    static const std::string    DEFAULT_ROOT;
    static const std::string    DEFAULT_INDEX;
};

#endif
