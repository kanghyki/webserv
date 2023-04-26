#include "./Http.hpp"
#include "HttpResponseBuilder.hpp"
#include "HttpStatus.hpp"
#include <sys/select.h>
#include <unistd.h>

Http::Http(ServerConfig config): config(config) {}

Http::~Http() {}

HttpResponse Http::processing(std::string s) {
  HttpResponse ret;

  try {
    HttpRequest req(s);

    if (req.getMethod() == request_method::GET) ret = getMethod(req);
    else if (req.getMethod() == request_method::POST) ret = postMethod(req);
    else if (req.getMethod() == request_method::DELETE) ret = deleteMethod(req);
    else if (req.getMethod() == request_method::PUT) ret = putMethod(req);
  } catch (HttpStatus status) {
    ret = getErrorPage(status);
  }

  return ret;
}

HttpResponse Http::getMethod(HttpRequest& req) {
  std::cout << "GET" << std::endl;
  HttpDataFecther fetcher(req, this->config);
//  std::string data = fetcher.fetch();
  return HttpResponseBuilder::getBuilder()
    .statusCode(OK)
    .header("date", getNowStr())
    .body(fetcher.getData(), fetcher.getMimeType())
    .build();
}

HttpResponse Http::postMethod(HttpRequest& req) {
  std::cout << "POST" << std::endl;
  if (access(("." + req.getPath()).c_str(), F_OK) == 0) throw BAD_REQUEST;

  std::ofstream out("." + req.getPath(), std::ofstream::out);
  if (!out.is_open()) throw NOT_FOUND;

  out.write(req.getBody().c_str(), req.getBody().length());
  if (out.fail() || out.bad() || out.eof()) throw INTERNAL_SERVER_ERROR;

  return HttpResponseBuilder::getBuilder()
    .statusCode(CREATED)
    .header("date", getNowStr())
    .body(req.getBody(), req.getContentType())
    .build();
}

HttpResponse Http::deleteMethod(HttpRequest& req) {
  std::cout << "DELETE" << std::endl;
  DIR* dir = opendir(("." + req.getPath()).c_str());
  if (dir) {
    closedir(dir);
    throw BAD_REQUEST;
  }
  if (std::remove(("." + req.getPath()).c_str()) == -1) throw NOT_FOUND;
  return HttpResponseBuilder::getBuilder()
    .statusCode(OK)
    .header("date", getNowStr())
    .build();
}

HttpResponse Http::putMethod(HttpRequest& req) {
  std::cout << "PUT" << std::endl;
  DIR* dir = opendir(("." + req.getPath()).c_str());
  if (dir) {
    closedir(dir);
    throw BAD_REQUEST;
  }
  std::ofstream out("." + req.getPath(), std::ofstream::out);
  if (!out.is_open()) throw NOT_FOUND;

  out.write(req.getBody().c_str(), req.getBody().length());
  if (out.fail() || out.bad() || out.eof()) throw INTERNAL_SERVER_ERROR;

  return HttpResponseBuilder::getBuilder()
    .statusCode(OK)
    .header("date", getNowStr())
    .body(req.getBody(), req.getContentType())
    .build();
}

HttpResponse Http::getErrorPage(HttpStatus status) {
  std::string                                 data;
  std::map<int, std::string>                  m = this->config.getErrorPage();
  std::map<int, std::string>::const_iterator  it;
  std::string                                 path;
 
  if ((it = m.find(status)) != m.end()) {
    path = it->second;
    data = HttpDataFecther::readFile(path);
  }

  return HttpResponseBuilder::getBuilder()
    .statusCode(status)
    .header("date", getNowStr())
    .body(data, util::getMimeType("." +path))
    .build();
}
