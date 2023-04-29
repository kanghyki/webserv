#include "./Http.hpp"

Http::Http() {}

Http::~Http() {}

HttpResponse Http::processing(const HttpRequest& req) throw(HttpStatus) {
  HttpResponse ret;

  HttpRequest::SessionStatus ss = req.getSessionStatus();

  if (ss == HttpRequest::EXPIRED) {
    req.getSession().erase(req.getSessionKey());
    ret.setStatusCode(FORBIDDEN);
    ret.addHeader(header_field::CONTENT_TYPE, "text/html");
    ret.setBody("<html><h1>Session expired!</h1></html>");
    return ret;
  }
  else if (ss == HttpRequest::NORMAL) {
    ret.setStatusCode(OK);
    ret.addHeader(header_field::CONTENT_TYPE, "text/html");
    ret.setBody("<html><h1>Session is work!</h1></html>");
    return ret;
  }
  else if (ss == HttpRequest::COOKIE_NOT_EXIST) {
    std::string random = HttpRequest::generateRandomString(15);
    req.getSession().insert(std::make_pair(random, time(0)));
    ret.setStatusCode(FORBIDDEN);
    ret.addHeader(header_field::CONTENT_TYPE, "text/html");
    ret.addHeader("Set-Cookie", HttpRequest::SESSION_KEY + "=" + random);
    ret.setBody("<html><h1>No cookie!</h1>\nnew cookie : " + random + "</html>");
    return ret;
  }
  else if (ss == HttpRequest::SESSION_NOT_EXIST) {
    std::string random = HttpRequest::generateRandomString(15);
    req.getSession().insert(std::make_pair(random, time(0)));
    ret.setStatusCode(FORBIDDEN);
    ret.addHeader(header_field::CONTENT_TYPE, "text/html");
    ret.addHeader("Set-Cookie", "_webserv_session=" + random);
    ret.setBody("<html><h1>No session!</h1>\nnew cookie : " + random + "</html>");
    return ret;
  }

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
  std::string body;
  std::map<std::string, std::string> header;

  try {
    CGI cgi(req, reads, fdMax);
    str = cgi.execute();
    std::pair<std::string, std::string> p = util::splitTwo(str, CRLF + CRLF);
    header = util::parseCGIHeader(p.first);
    body = p.second;
    std::cout << "header : " << p.first << std::endl;
    std::cout << "body : " << body << std::endl;
  } catch (HttpStatus status) {
    ret = getErrorPage(status, req.getLocationConfig());
  }

  ret.setStatusCode(OK);
  ret.addHeader(header_field::CONTENT_TYPE, header.at("content-type"));
  ret.setBody(body);

  return ret;
}

HttpResponse Http::getMethod(const HttpRequest& req) {
  HttpResponse res;

  std::cout << "GET" << std::endl;
  HttpDataFecther fetcher(req);
  std::string data = fetcher.fetch();

  res.setStatusCode(OK);
  res.addHeader(header_field::CONTENT_TYPE, req.getContentType());
  res.setBody(data);

  return res;
}

HttpResponse Http::postMethod(const HttpRequest& req) {
  HttpResponse res;

  std::cout << "POST" << std::endl;
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

  std::cout << "DELETE" << std::endl;
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

  res.setStatusCode(CREATED);
  res.addHeader(header_field::CONTENT_TYPE, req.getContentType());
  res.setBody(req.getBody());

  return res;
}

HttpResponse Http::getErrorPage(HttpStatus status, const LocationConfig& config) {
  std::string                                 data;
  std::map<int, std::string>                  m = config.getErrorPage();
  std::map<int, std::string>::const_iterator  it;
  std::string                                 path;
  HttpResponse                                res;

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
  res.setStatusCode(CREATED);
  res.addHeader(header_field::CONTENT_TYPE, "text/html");
  res.setBody(data);

  return res;
}
