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
    int                         getClientBodyTimeout() const;
    int                         getClientHeaderTimeout() const;
    int                         getSendTimeout() const;
    std::string                 getRoot() const;
    std::map<int, std::string>  getErrorPage() const;
    std::vector<std::string>    getIndex() const;

    void                        setClientBodySize(int n);
    void                        setClientBodyTimeout(int n);
    void                        setClientHeaderTimeout(int n);
    void                        setSendTimeout(int n);
    void                        setRoot(std::string root);
    void                        addErrorPage(int statusCode, std::string path);
    void                        addIndex(std::string index);

  protected:
    int                         clientBodySize;
    int                         clientBodyTimeout;
    int                         clientHeaderTimeout;
    int                         sendTimeout;
    std::string                 root;
    std::vector<std::string>    index;
    std::map<int, std::string>  errorPage;

  private:
    static const int            DEFAULT_CLIENT_BODY_SIZE;
    static const int            DEFAULT_CLIENT_BODY_TIMEOUT;
    static const int            DEFAULT_CLIENT_HEADER_TIMEOUT;
    static const int            DEFAULT_SEND_TIMEOUT;
    static const std::string    DEFAULT_ROOT;
    static const std::string    DEFAULT_INDEX;
};

#endif
