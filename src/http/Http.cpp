#include "./Http.hpp"
#include "HttpStatus.hpp"

Http::Http() {}

Http::~Http() {}

HttpResponse Http::processing(const HttpRequest& req, SessionManager& manager) {
  HttpResponse ret;

  if (req.getRecvStatus() == HttpRequest::ERROR)
    throw req.getErrorStatusCode();
  if (req.getLocationConfig().getReturnRes().first != -1) {
    ret.setStatusCode(static_cast<HttpStatus>(req.getLocationConfig().getReturnRes().first));
    ret.getHeader().set("Location", req.getLocationConfig().getReturnRes().second);
    return ret;
  }
  if (req.getBody().size() > static_cast<size_t>(req.getLocationConfig().getClientMaxBodySize()))
    throw (PAYLOAD_TOO_LARGE);
  if (req.getLocationConfig().isMethodAllowed(req.getMethod()) == false)
    throw (METHOD_NOT_ALLOWED);
  if (req.getMethod() != request_method::GET &&
      req.getMethod() != request_method::POST &&
      req.getMethod() != request_method::DELETE &&
      req.getMethod() != request_method::PUT &&
      req.getMethod() != request_method::HEAD)
    throw (METHOD_NOT_ALLOWED);

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

HttpResponse Http::executeCGI(const HttpRequest& req, SessionManager& sm) {
  std::string cgi_ret;
  HttpResponse ret;
  std::string body;
  std::map<std::string, std::string> header;

  try {
    std::map<std::string, std::string> c = util::splitHeaderField(req.getHeader().get(HttpRequestHeader::COOKIE));
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
    std::cout << c[SessionManager::SESSION_KEY] << std::endl;
    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
    std::cout << sm.isSessionAvailable(c[SessionManager::SESSION_KEY]) << std::endl;
    CGI cgi(req, sm.isSessionAvailable(c[SessionManager::SESSION_KEY]));
    cgi_ret = cgi.execute();
    std::pair<std::string, std::string> p = util::splitHeaderBody(cgi_ret, CRLF + CRLF);
    header = util::parseCGIHeader(p.first);
    body = p.second;
    ret.setStatusCode(OK);
  } catch (std::exception& e) {
    throw INTERNAL_SERVER_ERROR;
  }

  // FIXME:
  for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); ++it) {
    ret.getHeader().set(it->first, it->second);

    std::string lower_first = util::toLowerStr(it->first);
    if (lower_first == "status") {
      std::vector<std::string> vs = util::split(it->second, ' ');
      if (vs.size() < 1) throw INTERNAL_SERVER_ERROR;
      ret.setStatusCode(static_cast<HttpStatus>(std::atoi(vs[0].c_str())));
    }
    else if (lower_first == "set-cookie")
      sm.addSession(it->second, req.getServerConfig().getSessionTimeout());

  }

  ret.getHeader().remove("status");
  ret.setBody(body);


  return ret;
}

HttpResponse Http::getMethod(const HttpRequest& req) {
  HttpResponse res;

  HttpDataFecther fetcher(req);
  std::string data = fetcher.fetch();

  res.setStatusCode(OK);
  res.getHeader().set(header_field::CONTENT_TYPE, req.getContentType());
  res.setBody(data);


  return res;
}

HttpResponse Http::postMethod(const HttpRequest& req) {
  HttpResponse res;

  std::ofstream out(req.getTargetPath(), std::ofstream::out);
  if (!out.is_open()) throw FORBIDDEN;

  out.write(req.getBody().c_str(), req.getBody().length());
  if (out.fail() || out.bad() || out.eof()) throw INTERNAL_SERVER_ERROR;

  res.setStatusCode(CREATED);
  res.getHeader().set(header_field::CONTENT_TYPE, req.getContentType());
  res.getHeader().set("Location", req.getServerConfig().getServerName() + ":" + util::itoa(req.getServerConfig().getPort()) + req.getSubstitutedPath() );

  res.setBody(req.getBody());

  return res;
}

HttpResponse Http::deleteMethod(const HttpRequest& req) {
  HttpResponse res;
  struct stat _stat;

  if (stat(req.getTargetPath().c_str(), &_stat) == 0) {
    if (S_ISDIR(_stat.st_mode))
      throw (FORBIDDEN);
  }

  if (std::remove(req.getTargetPath().c_str()) == -1)
    throw NOT_FOUND;

  res.setStatusCode(OK);

  return res;
}

HttpResponse Http::putMethod(const HttpRequest& req) {
  HttpResponse res;
  struct stat _stat;

  if (stat(req.getTargetPath().c_str(), &_stat) == 0) {
    if (S_ISDIR(_stat.st_mode))
      throw (FORBIDDEN);
  }

  std::ofstream out(req.getTargetPath(), std::ofstream::out);
  if (!out.is_open()) throw NOT_FOUND;

  out.write(req.getBody().c_str(), req.getBody().length());
  if (out.fail() || out.bad() || out.eof()) throw INTERNAL_SERVER_ERROR;

  res.setStatusCode(NO_CONTENT);

  return res;
}

HttpResponse Http::headMethod(const HttpRequest& req) {
  HttpResponse res;

  HttpDataFecther fetcher(req);
  std::string data = fetcher.fetch();

  res.setStatusCode(NO_CONTENT);

  return res;
}

HttpResponse Http::getErrorPage(HttpStatus status, const LocationConfig& config) {
  HttpResponse  res;
  std::string   data;
  std::string   path;

  std::string errorPagePath = config.getErrorPage()[status];
  log::error << "error path path : " << errorPagePath << log::endl;
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
  res.getHeader().set(header_field::CONTENT_TYPE, "text/html");
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
