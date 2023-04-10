#ifndef COMMON_CONFIG_HPP
#define COMMON_CONFIG_HPP

#include <string>
#include <map>
#include <vector>

class CommonConfig {
  public:
    CommonConfig();
    ~CommonConfig();
    CommonConfig(const CommonConfig &obj);
    CommonConfig &operator=(const CommonConfig &obj);

    int getClientBodySize() const;
    void setClientBodySize(int clientBodySize);
    std::string getRoot() const;
    void setRoot(std::string root);
    std::map<int, std::string> getErrorPage() const;
    void addErrorPage(std::pair<int, std::string> errorPage);
    std::vector<std::string> getIndex() const;
    void addIndex(std::string index);

  private:
    int clientBodySize;
    std::string root;
    std::map<int, std::string> errorPage;
    std::vector<std::string> index;
};

#endif
