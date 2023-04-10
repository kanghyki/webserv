#ifndef COMMON_CONFIG_HPP
#define COMMON_CONFIG_HPP

#include <string>
#include <map>
#include <vector>

class CommonConfig {
  public:
    CommonConfig();
    virtual ~CommonConfig();
    CommonConfig(const CommonConfig &obj);
    CommonConfig &operator=(const CommonConfig &obj);

    int getClientBodySize() const;
    std::string getRoot() const;
    std::map<int, std::string> getErrorPage() const;
    std::vector<std::string> getIndex() const;

    void setClientBodySize(int clientBodySize);
    void setRoot(std::string root);
    void addErrorPage(std::pair<int, std::string> errorPage);
    void addIndex(std::string index);

  protected:
    int clientBodySize;
    std::string root;
    std::map<int, std::string> errorPage;
    std::vector<std::string> index;
};

#endif
