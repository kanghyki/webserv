#include "./HttpRequestTest.hpp"

void HttpRequestTest::test() {
  std::string header = "\
GET /html/index.html?name=kanghyki HTTP/1.1\r\n\
Host: localhost:13744\r\n\
Connection: keep-alive\r\n\
Content-Length: 123";

  HttpRequest hr;
  hr.parseHeader(header);

  AssertEqual(std::string(""), hr.getBody());
  AssertEqual(std::string("/html/index.html"), hr.getPath());
  AssertEqual(std::string("name=kanghyki"), hr.getQueryString());

  AssertEqual(std::string("localhost:13744"),hr.getField("host"));
  AssertEqual(std::string("keep-alive"),hr.getField("connection"));
  AssertEqual(std::string("123"), hr.getField("content-length"));
}

void HttpRequestTest::test1() {
  std::string header = "\
GET /html/cgi-bin?name=kanghyki&drink=coffee HTTP/1.1\r\n\
Host: localhost:13744\r\n\
Connection: keep-alive\r\n\
Content-Length: 123";

  HttpRequest hr;
  try {
  hr.parseHeader(header);
  } catch (HttpStatus s) {
    std::cout << s << std::endl;
  }

  AssertEqual(std::string(""), hr.getBody());
  AssertEqual(std::string("/html/cgi-bin"), hr.getPath());
  AssertEqual(std::string("name=kanghyki&drink=coffee"), hr.getQueryString());

  AssertEqual(std::string("localhost:13744"),hr.getField("host"));
  AssertEqual(std::string("keep-alive"),hr.getField("connection"));
  AssertEqual(std::string("123"), hr.getField("content-length"));
}
