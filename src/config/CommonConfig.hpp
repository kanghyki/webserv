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

    int                         getClientMaxBodySize() const;
    std::string                 getRoot() const;
    std::map<int, std::string>  getErrorPage() const;
    std::string                 getIndex() const;

    std::string                 getErrorPageTargetPath(int status) const;

    void                        setClientMaxBodySize(int n);
    void                        setRoot(std::string root);
    void                        addErrorPage(int statusCode, std::string path);
    void                        setIndex(std::string index);

  protected:
    int                         clientMaxBodySize;
    std::string                 root;
    std::string                 index;
    std::map<int, std::string>  errorPage;

  private:
    static const int            DEFAULT_CLIENT_BODY_SIZE;
    static const std::string    DEFAULT_ROOT;
    static const std::string    DEFAULT_INDEX;

    std::string                 trimChar(std::string s, char c) const;
    std::string                 combinePath(const std::string& s1, const std::string& s2) const;
};

#endif
