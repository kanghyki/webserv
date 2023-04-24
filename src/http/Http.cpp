#include "./Http.hpp"
#include "HttpResponseBuilder.hpp"
#include <sys/select.h>
#include <unistd.h>

Http::Http(ServerConfig config): config(config) {}

Http::~Http() {}

std::string Http::processing(HttpRequest& request) {
  std::string ret;

  try {
    //HttpRequest request(s);
    HttpDataFecther fetcher(request, this->config);
    std::string data = fetcher.fetch();
    ret = HttpResponseBuilder::getBuilder()
      .statusCode(OK)
      .httpVersion("HTTP/1.1")
      .header("date", getNowStr())
      .body(data)
      .build()
      .toString();
  } catch (HttpStatus status) {
    ret = getErrorPage(status);
  }
  return ret;
}

std::string Http::getErrorPage(HttpStatus status) {
  std::string ret;
  std::string data;
  std::vector<int> sl = this->config.getErrorPageStatus();
  std::vector<int>::iterator it = std::find(sl.begin(), sl.end(), status);
 
  if (it != sl.end())
    data = HttpDataFecther::readFile(this->config.getErrorPagePath());

  ret = HttpResponseBuilder::getBuilder()
    .statusCode(status)
    .httpVersion()
    .header("date", getNowStr())
    .body(data)
    .build()
    .toString();

  return ret;
}
