#include "./HttpStatus.hpp"

std::string getStatusText(const HttpStatus statusCode) {
  std::string ret;

  switch (statusCode) {
    case 100:
      ret = "Continue";
    case 101:
      ret = "Switching Protocols";
    case 200:
      ret = "OK";
    case 201:
      ret = "Created";
    case 202:
      ret = "Accepted";
    case 203:
      ret = "Non-Authoritative Information";
    case 204:
      ret = "No Content";
    case 205:
      ret = "Reset Content";
    case 206:
      ret = "Partial Content";
    case 300:
      ret = "Multiple Choices";
    case 301:
      ret = "Moved Permanently";
    case 302:
      ret = "Found";
    case 303:
      ret = "See Other";
    case 304:
      ret = "Not Modified";
    case 305:
      ret = "Use Proxy";
    case 307:
      ret = "Temporary Redirect";
    case 400:
      ret = "Bad Request";
    case 401:
      ret = "Unauthorized";
    case 402:
      ret = "Payment Required";
    case 403:
      ret = "Forbidden";
    case 404:
      ret = "Not Found";
    case 405:
      ret = "Method Not Allowed";
    case 406:
      ret = "Not Acceptable";
    case 407:
      ret = "Proxy Authentication Required";
    case 408:
      ret = "Request Timeout";
    case 409:
      ret = "Conflict";
    case 410:
      ret = "Gone";
    case 411:
      ret = "Length Required";
    case 412:
      ret = "Precondition Failed";
    case 413:
      ret = "Payload Too Large";
    case 414:
      ret = "URI Too Long";
    case 415:
      ret = "Unsupported Media Type";
    case 416:
      ret = "Range Not Satisfiable";
    case 417:
      ret = "Expectation Failed";
    case 426:
      ret = "Upgrade Required";
    case 500:
      ret = "Internal Server Error";
    case 501:
      ret = "Not Implemented";
    case 502:
      ret = "Bad Gateway";
    case 503:
      ret = "Service Unavailable";
    case 504:
      ret = "Gateway Timeout";
    case 505:
      ret = "HTTP Version Not Supported";
  }
  return ret;
}
