#include "./UriParser.hpp"

UriParser::UriParser(const std::string& req) {
}

UriParser::~UriParser() {}

std::string UriParser::getPath() {
  return this->path;
}

std::map<std::string, std::string> UriParser::getQuery() {
  return this->query;
}

void UriParser::parseQuery(const std::string& query) {
  std::vector<std::string> ss = util::split(query, '&');

  for (int i = 0; i < ss.size(); ++i) {
    std::vector<std::string> sss = util::split(ss[i], '=');
    if (sss.size() == 1)
      this->query.insert(std::make_pair(sss[0], ""));
    else
      this->query.insert(std::make_pair(sss[0], sss[1]));
  }
}
