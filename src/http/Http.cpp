#include "./Http.hpp"
#include "HttpHeaderField.hpp"
#include <exception>

Http::Http() {}

Http::~Http() {}

std::string generateRandomString(int ch)
{
  srand(time(0));
  char alpha[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g',
    'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u',
    'v', 'w', 'x', 'y', 'z' };
  std::string result = "";
  for (int i = 0; i<ch; i++)
    result = result + alpha[rand() % 26];

  return result;
}

HttpResponse Http::processing(const HttpRequest req) throw(HttpStatus) {
  HttpResponse ret;
//  if (req.getPath() == "/favicon.ico")
//    return HttpResponseBuilder::getBuilder()
//      .statusCode(OK)
//      .build();
//
//  static std::map<std::string, time_t> session;
//
//  const std::string sessionKey = "_webserv_session";
//
//  std::string cookies = req.getField("Cookie");
//
//  std::cout << "Cookies: " << cookies << std::endl;
//
//  std::map<std::string, std::string> cookieMap;
//  std::vector<std::string> vs = util::split(cookies, ';');
//
//  std::cout << "vs size: " << vs.size() << std::endl;
//
//  for (std::vector<std::string>::iterator it = vs.begin(); it != vs.end(); ++it) {
//    std::vector<std::string> vvs = util::split(*it, '=');
//
//    std::string key, value;
//    key = util::trimSpace(vvs[0]);
//    value = util::trimSpace(vvs[1]);
//
//    std::cout << "makecookie:" << key << "=" << value << std::endl;
//
//    cookieMap.insert(std::make_pair(key, value));
//  }
//  std::map<std::string, std::string>::iterator cit = cookieMap.find(sessionKey);
//  std::cout << "Cookiemap size: " << cookieMap.size() << std::endl;
//  if (cit != cookieMap.end()) { // 세션 쿠키 찾음
//    std::cout << "[ COOKIE FIND key=" << cit->first << ", value=" << cit->second << " ]" << std::endl;
//    std::cout << "[ COOKIE FIND key=" << cit->first << ", value=" << cit->second << " ]" << std::endl;
//    std::cout << "[ COOKIE FIND key=" << cit->first << ", value=" << cit->second << " ]" << std::endl;
//    std::map<std::string, time_t>::iterator sit = session.find(cit->second);
//    if (sit != session.end()) {  // session 있음
//      std::cout << "[ SESSION FIND key=" << sit->first << ", value=" << sit->second << " ]" << std::endl;
//      std::cout << "[ SESSION FIND key=" << sit->first << ", value=" << sit->second << " ]" << std::endl;
//      std::cout << "[ SESSION FIND key=" << sit->first << ", value=" << sit->second << " ]" << std::endl;
//      time_t curTime = time(0);
//      int EXPIRED_TIME = 60;
//      std::cout << "curTime: " << curTime << std::endl;
//      std::cout << "sit: " << sit->second << std::endl;
//      if (curTime - sit->second > EXPIRED_TIME) { // 쿠키 만료
//        session.erase(cit->second); // session 삭제
//        return HttpResponseBuilder::getBuilder()
//          .statusCode(FORBIDDEN)
//          .body("<html><h1>Session expired!</h1></html>", "text/html")
//          .header("date", getNowStr())
//          .build();
//      }
//      else { // 정상
//        std::cout << "session good" << std::endl;
//        return HttpResponseBuilder::getBuilder()
//          .statusCode(OK)
//          .header("date", getNowStr())
//          .body("<html><h1>Session is work!!</h1></html>", "text/html")
//          .build();
//      }
//    }
//    else {
//      std::cout << "No session" << std::endl;
//    }
//  }
//  else {
//    std::cout << "[ SESSION COOKIE NOT FOUND ]" << std::endl;
//    std::cout << "[ SESSION COOKIE NOT FOUND ]" << std::endl;
//    std::cout << "[ SESSION COOKIE NOT FOUND ]" << std::endl;
//  }
//
//  // 쿠키 주기~!
//
//  std::string randomString  = generateRandomString(15);
//  time_t curTime = time(0);
//  session.insert(std::make_pair(randomString, curTime));
//  return HttpResponseBuilder::getBuilder()
//    .statusCode(OK)
//    .header("date", getNowStr())
//    .body("<html><h1>this is random string => " + randomString + "</h1></html>", "text/html")
//    .header("set-cookie", sessionKey + "=" + randomString)
//    .build();
//  // 세션 만료는 타임아웃처럼 처리
//  // 세션 테이블은 std::map<std::string(cookie), time_t(expired)>
//  // ㅅㅔ션 테이블 확인
//  // 있으면
//    // 그거에 맞게 데이터 가져오기 & 데이터 업데이트
//  // 없으면
//    // 새로운 세션 생성 및 쿠키 전송

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
