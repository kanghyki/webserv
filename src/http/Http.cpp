#include "./Http.hpp"
#include "HttpResponseBuilder.hpp"
#include <sys/select.h>
#include <unistd.h>

Http::Http(ServerConfig config): config(config) {}

Http::~Http() {}

HttpResponseBuilder Http::processing(std::string s) {
  HttpResponseBuilder ret;

  try {
    HttpRequest req(s);

    if (req.getMethod() == request_method::GET) ret = getMethod(req);
    else if (req.getMethod() == request_method::POST) ret = postMethod(req);
    else if (req.getMethod() == request_method::DELETE) ret = deleteMethod(req);
  } catch (HttpStatus status) {
    ret = getErrorPage(status);
  }

  return ret;
}

HttpResponseBuilder Http::getMethod(HttpRequest& req) {
  std::cout << "GET" << std::endl;
  HttpDataFecther fetcher(req, this->config);
  std::string data = fetcher.fetch();
  return HttpResponseBuilder::getBuilder()
    .statusCode(OK)
    .header("date", getNowStr())
    .body(data);
}

HttpResponseBuilder Http::postMethod(HttpRequest& req) {
  std::cout << "POST" << std::endl;
  return HttpResponseBuilder::getBuilder();
}

#include <fstream>
HttpResponseBuilder Http::deleteMethod(HttpRequest& req) {
  std::cout << "DELETE" << std::endl;
  if (std::remove(("." + req.getPath()).c_str()) == 0) {
    return HttpResponseBuilder::getBuilder()
      .statusCode(OK)
      .header("date", getNowStr());
  }
  return HttpResponseBuilder::getBuilder()
      .statusCode(NOT_FOUND)
      .header("date", getNowStr());
}

HttpResponseBuilder Http::getErrorPage(HttpStatus status) {
  std::string ret;
  std::string data;
  std::vector<int> sl = this->config.getErrorPageStatus();
  std::vector<int>::iterator it = std::find(sl.begin(), sl.end(), status);
 
  if (it != sl.end())
    data = HttpDataFecther::readFile(this->config.getErrorPagePath());

  return HttpResponseBuilder::getBuilder()
    .statusCode(status)
    .header("date", getNowStr())
    .body(data);
}
