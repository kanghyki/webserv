#include "./HttpResponseTest.hpp"

void HttpResponseTest::response_toString_test() {
  std::string html = "\
<html>\
<body>\
<h1>Hello, World! This is Wevserv by hyeongki and kanghyki!</h1>\
</body>\
</html>";
  std::string html_size = std::to_string(html.length());



  HttpResponseBuilder rb;

  HttpResponse hr = rb.statusCode(OK)
    .header("content-type", "text/plain")
    .header("cache-control", "no-cache")
    .header("date", "Tue, 25 Apr 2023 09:22:32 GMT")
    .body(html)
    .build();

  std::string expect = "\
HTTP/1.1 200 OK\r\n\
Content-Length: " + html_size + "\r\n\
cache-control: no-cache\r\n\
content-type: text/plain\r\n\
date: Tue, 25 Apr 2023 09:22:32 GMT\r\n\
\r\n"
  + html;


  AssertEqual(expect, hr.toString());
}

void HttpResponseTest::response_toString_test2() {
  std::string html = "\
<html>\
<body>\
<h1>Hello, World! This is Wevserv by hyeongki and kanghyki!</h1>\
</body>\
</html>";
  std::string html_size = std::to_string(html.length());



  HttpResponseBuilder rb;

  HttpResponse hr = rb.statusCode(METHOD_NOT_ALLOWED)
    .header("content-type", "text/plain")
    .header("allow-method", "GET, POST, DELETE, PUT")
    .header("cache-control", "no-cache")
    .header("date", "Tue, 25 Apr 2023 09:22:32 GMT")
    .build();

  std::string expect = "\
HTTP/1.1 405 Method Not Allowed\r\n\
Content-Length: 0\r\n\
allow-method: GET, POST, DELETE, PUT\r\n\
cache-control: no-cache\r\n\
content-type: text/plain\r\n\
date: Tue, 25 Apr 2023 09:22:32 GMT\r\n\
\r\n";

  AssertEqual(expect, hr.toString());
}
