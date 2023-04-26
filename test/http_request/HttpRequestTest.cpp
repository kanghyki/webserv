#include "./HttpRequestTest.hpp"

void HttpRequestTest::test() {
  std::string header = "\
GET /html/index.html HTTP/1.1\r\n\
Host: localhost:13744\r\n\
Connection: keep-alive\r\n\
Upgrade-Insecure-Requests: 1\r\n\
Content-Length: 123\r\n\
Sec-Fetch-Site: none\r\n\
Sec-Fetch-Mode: navigate\r\n\
Sec-Fetch-User: ?1\r\n\
Sec-Fetch-Dest: document\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
Accept-Language: en-US,en;q=0.9";
  HttpRequest hr;
  
  hr.parseHeader(header);

  AssertEqual(std::string(""), hr.getBody());

  AssertEqual(std::string("/html/index.html"), hr.getPath());

  std::string expect1 = "123";
  AssertEqual(expect1, hr.getField("content-length"));
}
