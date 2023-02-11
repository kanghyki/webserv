#include "http.hpp"

Http::Http(std::string header_str) : header_str(header_str) {
  ConvertHeader(header_str);
}

void  Http::ConvertHeader(std::string header_str) {
  std::vector<std::string> tmp = util::Split(header_str, '\n');
  std::vector<std::string>::iterator tmp_iter = tmp.begin();
  std::vector<std::string> first = util::Split(*tmp_iter++, ' ');
  std::vector<std::string>::iterator first_iter = first.begin();
  this->header.insert(std::pair<std::string, std::string>("metohd", *first_iter++));
  this->header.insert(std::pair<std::string, std::string>("url", *first_iter++));
  this->header.insert(std::pair<std::string, std::string>("version", *first_iter));
  for (;tmp_iter != tmp.end(); ++tmp_iter) {
    std::vector<std::string> line = util::Split(*tmp_iter, ':');
    this->header.insert(std::pair<std::string, std::string>(line[0], line[1]));
  }
  std::map<std::string, std::string>::iterator header_iter = this->header.begin();
  for (;header_iter != this->header.end(); ++header_iter)
    std::cout << header_iter->first << " " << header_iter->second << std::endl;
}

//void  Http::ConvertHeader(std::map<std::string, std::string> header) {
//
//}
