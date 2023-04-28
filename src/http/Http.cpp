#include "./Http.hpp"
#include "HttpResponseBuilder.hpp"
#include "HttpStatus.hpp"
#include <sys/select.h>
#include <unistd.h>

Http::Http() {}

Http::~Http() {}

HttpResponse Http::processing(const HttpRequest req) throw(HttpStatus) {
  HttpResponse ret;

  std::cout << req.getRelativePath() << std::endl;
  try {
    if (req.getMethod() == request_method::GET) ret = getMethod(req);
    else if (req.getMethod() == request_method::POST) ret = postMethod(req);
    else if (req.getMethod() == request_method::DELETE) ret = deleteMethod(req);
    else if (req.getMethod() == request_method::PUT) ret = putMethod(req);
  } catch (HttpStatus status) {
    ret = getErrorPage(status, req.getLocationConfig());
  }

  return ret;
}

HttpResponse Http::executeCGI(const HttpRequest& req, fd_set& reads, int& fdMax) throw (HttpStatus) {
  std::string str;
  HttpResponse ret;

  try {
    CGI cgi(req, reads, fdMax);
    str = cgi.execute();
  } catch (HttpStatus status) {
    ret = getErrorPage(status, req.getLocationConfig());
  }

  ret = HttpResponseBuilder::getBuilder()
    .statusCode(OK)
    .header("date", getNowStr())
    .body(str, req.getContentType())
    .build();

  return ret;
}

HttpResponse Http::getMethod(const HttpRequest& req) {
  std::cout << "GET" << std::endl;
  HttpDataFecther fetcher(req);
  std::string data = fetcher.fetch();
  return HttpResponseBuilder::getBuilder()
    .statusCode(OK)
    .header("date", getNowStr())
    .body(data, req.getContentType())
    .build();
}

HttpResponse Http::postMethod(const HttpRequest& req) {
  std::cout << "POST" << std::endl;
  if (access(req.getRelativePath().c_str(), F_OK) == 0) throw BAD_REQUEST;

  std::ofstream out(req.getRelativePath(), std::ofstream::out);
  if (!out.is_open()) throw NOT_FOUND;

  out.write(req.getBody().c_str(), req.getBody().length());
  if (out.fail() || out.bad() || out.eof()) throw INTERNAL_SERVER_ERROR;

  return HttpResponseBuilder::getBuilder()
    .statusCode(CREATED)
    .header("date", getNowStr())
    .body(req.getBody(), req.getContentType())
    .build();
}

HttpResponse Http::deleteMethod(const HttpRequest& req) {
  std::cout << "DELETE" << std::endl;
  DIR* dir = opendir(req.getRelativePath().c_str());
  if (dir) {
    closedir(dir);
    throw BAD_REQUEST;
  }
  if (std::remove(req.getRelativePath().c_str()) == -1) throw NOT_FOUND;
  return HttpResponseBuilder::getBuilder()
    .statusCode(OK)
    .header("date", getNowStr())
    .build();
}

HttpResponse Http::putMethod(const HttpRequest& req) {
  std::cout << "PUT" << std::endl;
  DIR* dir = opendir(req.getRelativePath().c_str());
  if (dir) {
    closedir(dir);
    throw BAD_REQUEST;
  }
  std::ofstream out(req.getRelativePath(), std::ofstream::out);
  if (!out.is_open()) throw NOT_FOUND;

  out.write(req.getBody().c_str(), req.getBody().length());
  if (out.fail() || out.bad() || out.eof()) throw INTERNAL_SERVER_ERROR;

  return HttpResponseBuilder::getBuilder()
    .statusCode(OK)
    .header("date", getNowStr())
    .body(req.getBody(), req.getContentType())
    .build();
}

HttpResponse Http::getErrorPage(HttpStatus status, const LocationConfig& config) {
  std::string                                 data;
  std::map<int, std::string>                  m = config.getErrorPage();
  std::map<int, std::string>::const_iterator  it;
  std::string                                 path;

  std::cout << "Http error occured: " << status << std::endl;
  if ((it = m.find(status)) != m.end()) {
    path = "." + it->second;
    try {
      data = HttpDataFecther::readFile(path);
    } catch (HttpStatus status) {
      std::cout << "ERROR OF ERROR" << std::endl;
    }
  }

  // FIXME: erorr of error?
  return HttpResponseBuilder::getBuilder()
    .statusCode(status)
    .header("date", getNowStr())
    .body(data, "text/html")
    .build();
}
