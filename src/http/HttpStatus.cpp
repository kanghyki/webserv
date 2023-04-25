#include "./HttpStatus.hpp"

std::string getStatusText(const HttpStatus statusCode) {
  std::string ret;

  switch (statusCode) {
    case 0:
      ret = "Undefined";
      break;
    case 100:
      ret = "Continue";
      break;
    case 101:
      ret = "Switching Protocols";
      break;
    case 200:
      ret = "OK";
      break;
    case 201:
      ret = "Created";
      break;
    case 202:
      ret = "Accepted";
      break;
    case 203:
      ret = "Non-Authoritative Information";
      break;
    case 204:
      ret = "No Content";
      break;
    case 205:
      ret = "Reset Content";
      break;
    case 206:
      ret = "Partial Content";
      break;
    case 300:
      ret = "Multiple Choices";
      break;
    case 301:
      ret = "Moved Permanently";
      break;
    case 302:
      ret = "Found";
      break;
    case 303:
      ret = "See Other";
      break;
    case 304:
      ret = "Not Modified";
      break;
    case 305:
      ret = "Use Proxy";
      break;
    case 307:
      ret = "Temporary Redirect";
      break;
    case 400:
      ret = "Bad Request";
      break;
    case 401:
      ret = "Unauthorized";
      break;
    case 402:
      ret = "Payment Required";
      break;
    case 403:
      ret = "Forbidden";
      break;
    case 404:
      ret = "Not Found";
      break;
    case 405:
      ret = "Method Not Allowed";
      break;
    case 406:
      ret = "Not Acceptable";
      break;
    case 407:
      ret = "Proxy Authentication Required";
      break;
    case 408:
      ret = "Request Timeout";
      break;
    case 409:
      ret = "Conflict";
      break;
    case 410:
      ret = "Gone";
      break;
    case 411:
      ret = "Length Required";
      break;
    case 412:
      ret = "Precondition Failed";
      break;
    case 413:
      ret = "Payload Too Large";
      break;
    case 414:
      ret = "URI Too Long";
      break;
    case 415:
      ret = "Unsupported Media Type";
      break;
    case 416:
      ret = "Range Not Satisfiable";
      break;
    case 417:
      ret = "Expectation Failed";
      break;
    case 426:
      ret = "Upgrade Required";
      break;
    case 500:
      ret = "Internal Server Error";
      break;
    case 501:
      ret = "Not Implemented";
      break;
    case 502:
      ret = "Bad Gateway";
      break;
    case 503:
      ret = "Service Unavailable";
      break;
    case 504:
      ret = "Gateway Timeout";
      break;
    case 505:
      ret = "HTTP Version Not Supported";
      break;
  }
  return ret;
}
