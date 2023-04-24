#include "./Http.hpp"
#include <sys/select.h>
#include <unistd.h>

Http::Http(ServerConfig config): config(config) {}

Http::~Http() {}

std::string Http::processing(std::string s) {
  std::string ret;

  //////// parse----------------------------------
  HttpRequest request(s);
  if (request.isError()) {
    //!!!!!! parse-error----------------------------
    std::cout << "REQUEST ERROR" << std::endl;
    std::cout << request.getErrorStatus() << std::endl;

    std::string data;
    std::vector<int> sl = this->config.getErrorPageStatus();
    std::vector<int>::iterator it = std::find(sl.begin(), sl.end(), 404);
    if (it != sl.end()) {
      data = HttpDataFecther::readFile(this->config.getErrorPagePath());
    }
//    std::map<int, std::string> ep = this->config.getErrorPage();
//    if (ep.find(404) != ep.end()) {
//    }
    ret = HttpResponseBuilder::getBuilder()
      .statusCode(NOT_FOUND)
      .httpVersion("HTTP/1.1")
      .header("date", getNowStr())
      .body(data)
      .build()
      .toString();
  }
  //////// parse-end------------------------------
  else {
    //////// fetch----------------------------------
    HttpDataFecther fetcher(request, this->config);
    try {
      std::string data = fetcher.fetch();
      ret = HttpResponseBuilder::getBuilder()
        .statusCode(OK)
        .httpVersion("HTTP/1.1")
        .header("date", getNowStr())
        .body(data)
        .build()
        .toString();
    } catch (std::exception &e) {
      //!!!!!! fetch-error----------------------------
      std::cout << "FETCH ERROR" << std::endl;
      std::string data;
      std::vector<int> sl = this->config.getErrorPageStatus();
      std::vector<int>::iterator it = std::find(sl.begin(), sl.end(), 404);
      if (it != sl.end()) {
        data = HttpDataFecther::readFile(this->config.getErrorPagePath());
      }
      ret = HttpResponseBuilder::getBuilder()
        .statusCode(NOT_FOUND)
        .httpVersion("HTTP/1.1")
        .header("date", getNowStr())
        .body(data)
        .build()
        .toString();
    }
    //////// fetch-end------------------------------
  }
  return ret;
}
