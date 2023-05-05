#include "./Http.hpp"
#include "HttpHeaderField.hpp"
#include <exception>

Http::Http() {}

Http::~Http() {}

HttpResponse Http::processing(const HttpRequest& req, SessionManager& manager) throw(HttpStatus) {
  HttpResponse ret;

  if (req.getLocationConfig().getReturnRes().first != -1) {
    ret.setStatusCode(static_cast<HttpStatus>(req.getLocationConfig().getReturnRes().first));
    ret.addHeader("Location", req.getLocationConfig().getReturnRes().second);
    return ret;
  }
  log::debug << log::endl << "Relative path: " + req.getRelativePath() << log::endl;
  if (req.getBody().size() > static_cast<size_t>(req.getLocationConfig().getClientBodySize()))
    throw (PAYLOAD_TOO_LARGE);
  if (req.getLocationConfig().isMethodAllowed(req.getMethod()) == false)
    throw (METHOD_NOT_ALLOWED);
  if (req.getMethod() != request_method::GET &&
      req.getMethod() != request_method::POST &&
      req.getMethod() != request_method::DELETE &&
      req.getMethod() != request_method::PUT &&
      req.getMethod() != request_method::HEAD)
    throw (METHOD_NOT_ALLOWED);

  log::debug << log::endl << "Http -------------\n\
Request Path: " << req.getPath() << "\n\
Method: " << req.getMethod() << log::endl;


  try {
    if (req.isCGI()) ret = executeCGI(req, manager);
    else if (req.getMethod() == request_method::GET) ret = getMethod(req);
    else if (req.getMethod() == request_method::POST) ret = postMethod(req);
    else if (req.getMethod() == request_method::DELETE) ret = deleteMethod(req);
    else if (req.getMethod() == request_method::PUT) ret = putMethod(req);
    else if (req.getMethod() == request_method::HEAD) ret = headMethod(req);
  } catch (HttpStatus status) {
    ret = getErrorPage(status, req.getLocationConfig());
  }

  return ret;
}

HttpResponse Http::executeCGI(const HttpRequest& req, SessionManager& sm) throw (HttpStatus) {
  std::string str;
  HttpResponse ret;
  std::string body;
  std::map<std::string, std::string> header;

  try {
    std::map<std::string, std::string> c = util::splitHeaderField(req.getField(header_field::COOKIE));
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
    std::cout << c[SessionManager::SESSION_KEY] << std::endl;
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
    std::cout << sm.isSessionAvailable(c[SessionManager::SESSION_KEY]) << std::endl;
    CGI cgi(req, sm.isSessionAvailable(c[SessionManager::SESSION_KEY]));
    str = cgi.execute();
    std::pair<std::string, std::string> p = util::splitHeaderBody(str, CRLF + CRLF);
    header = util::parseCGIHeader(p.first);
    body = p.second;
  } catch (HttpStatus status) {
    ret = getErrorPage(status, req.getLocationConfig());
  }

  for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it) {
    ret.addHeader(it->first, it->second);
    if (it->first == "set-cookie")
      sm.addSession(it->second, req.getServerConfig().getSessionTimeout());
  }


  std::string sc = header[header_field::SET_COOKIE];
  ret.setStatusCode(OK);

  ret.setBody(body);

  return ret;
}

HttpResponse Http::getMethod(const HttpRequest& req) {
  HttpResponse res;

  HttpDataFecther fetcher(req);
  std::string data = fetcher.fetch();

  res.setStatusCode(OK);
  res.addHeader(header_field::CONTENT_TYPE, req.getContentType());
  res.setBody(data);


  return res;
}

HttpResponse Http::postMethod(const HttpRequest& req) {
  HttpResponse res;

  if (access(req.getRelativePath().c_str(), F_OK) == 0) throw BAD_REQUEST;

  std::ofstream out(req.getRelativePath(), std::ofstream::out);
  if (!out.is_open()) throw NOT_FOUND;

  out.write(req.getBody().c_str(), req.getBody().length());
  if (out.fail() || out.bad() || out.eof()) throw INTERNAL_SERVER_ERROR;

  res.setStatusCode(CREATED);
  res.addHeader(header_field::CONTENT_TYPE, req.getContentType());
  res.setBody(req.getBody());

  return res;
}

HttpResponse Http::deleteMethod(const HttpRequest& req) {
  HttpResponse res;

  DIR* dir = opendir(req.getRelativePath().c_str());
  if (dir) {
    closedir(dir);
    throw BAD_REQUEST;
  }
  if (std::remove(req.getRelativePath().c_str()) == -1) throw NOT_FOUND;

  res.setStatusCode(CREATED);

  return res;
}

HttpResponse Http::putMethod(const HttpRequest& req) {
  HttpResponse res;

  DIR* dir = opendir(req.getRelativePath().c_str());
  if (dir) {
    closedir(dir);
    throw BAD_REQUEST;
  }
  std::ofstream out(req.getRelativePath(), std::ofstream::out);
  if (!out.is_open()) throw NOT_FOUND;

  out.write(req.getBody().c_str(), req.getBody().length());
  if (out.fail() || out.bad() || out.eof()) throw INTERNAL_SERVER_ERROR;

  res.setStatusCode(CREATED);
  res.addHeader(header_field::CONTENT_TYPE, req.getContentType());
  res.setBody(req.getBody());

  return res;
}

HttpResponse Http::headMethod(const HttpRequest& req) {
  HttpResponse res;

  std::cout << "HEAD" << std::endl;
  HttpDataFecther fetcher(req);
  std::string data = fetcher.fetch();

  res.setStatusCode(OK);

  return res;
}

HttpResponse Http::getErrorPage(HttpStatus status, const LocationConfig& config) {
  HttpResponse  res;
  std::string   data;
  std::string   path;

  log::error << "Http error occured: " << status << log::endl;
  std::string errorPagePath = config.getErrorPage()[status];
  if (errorPagePath.empty())
    data = defaultErrorPage(status);
  else {
    path = "." + errorPagePath;
    try {
      data = HttpDataFecther::readFile(path);
    } catch (HttpStatus status) {
      data = defaultErrorPage(status);
    }
  }

  res.setStatusCode(status);
  res.addHeader(header_field::CONTENT_TYPE, "text/html");
  res.setBody(data);

  return res;
}

std::string Http::defaultErrorPage(HttpStatus s) {
  std::string ret = "<html><head><title>"\
+ util::itoa(s) + " " + getStatusText(s)\
+ "</title></head>\
<body>\
<center><h1>"\
+ util::itoa(s) + " " + getStatusText(s)\
+ "</h1></center>\
<hr><center>webserv/1.0.0</center>\
</body>\
</html>";

  return ret;
}
