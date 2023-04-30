#include "./Http.hpp"
#include "HttpHeaderField.hpp"
#include <exception>

Http::Http() {}

Http::~Http() {}

HttpResponse Http::processing(const HttpRequest& req) throw(HttpStatus) {
  HttpResponse ret;

  if (req.getBody().size() > static_cast<size_t>(req.getLocationConfig().getClientBodySize()))
    throw (PAYLOAD_TOO_LARGE);
  if (req.getLocationConfig().isMethodAllowed(req.getMethod()) == false)
    throw (METHOD_NOT_ALLOWED);
  if (req.getMethod() != request_method::GET &&
      req.getMethod() != request_method::POST &&
      req.getMethod() != request_method::DELETE &&
      req.getMethod() != request_method::PUT)
    throw (METHOD_NOT_ALLOWED);

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

HttpResponse Http::executeCGI(const HttpRequest& req) throw (HttpStatus) {
  std::string str;
  HttpResponse ret;
  std::string body;
  std::map<std::string, std::string> header;
  std::string ct;

  try {
    CGI cgi(req);
    str = cgi.execute();
    std::pair<std::string, std::string> p = util::splitHeaderBody(str, CRLF + CRLF);
    header = util::parseCGIHeader(p.first);
    body = p.second;
  } catch (HttpStatus status) {
    ret = getErrorPage(status, req.getLocationConfig());
  }
  try {
    ct = header.at("content-type");
  } catch (std::exception& e) {}

  ret.setStatusCode(OK);
  if (!ct.empty()) 
    ret.addHeader(header_field::CONTENT_TYPE, ct);
  else
    ret.addHeader(header_field::CONTENT_TYPE, util::itoa(body.length()));
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
  HttpResponse  res;
  std::string   data;
  std::string   path;

  std::cout << "Http error occured: " << status << std::endl;
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
