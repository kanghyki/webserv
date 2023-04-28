#include "./ConfigTest.hpp"

void ConfigTest::parseTest1() {
  ConfigParser cp;
  Config result = cp.parse("./test1.conf");
  ServerConfig sc = result.getHttpConfig()[0].getServerConfig()[0];

  short port = sc.getPort();
  std::string host = sc.getHost();
  std::string server_name = sc.getServerName();
  std::vector<std::string> index = sc.getIndex();
  int client_body_buffer_size = sc.getClientBodySize();
  std::string root = sc.getRoot();
  std::map<int, std::string> error_page = sc.getErrorPage();

  AssertEqual(port, (short)80);
  AssertEqual(host, std::string("localhost"));
  AssertEqual(server_name, std::string("kanghyki.42.fr"));
  std::vector<std::string> e1;
  e1.push_back("index.html");
  e1.push_back("index.htm");
  e1.push_back("index.php");
  std::vector<std::string>::iterator it = index.begin();
  std::vector<std::string>::iterator it2 = e1.begin();
  while (it != index.end()) {
    AssertEqual((*it), (*it2));
    ++it;
    ++it2;
  }
  AssertEqual(client_body_buffer_size, 30);
  AssertEqual(root, std::string("/"));
  std::map<int, std::string> expect2;
  expect2.insert(std::make_pair(404, "/html/error.html"));
  expect2.insert(std::make_pair(405, "/html/error.html"));
  expect2.insert(std::make_pair(500, "/html/serror.html"));
  AssertEqual(expect2.size(), error_page.size());
  for (auto it = expect2.begin(); it != expect2.end(); ++it) {
    std::string s;
    std::map<int, std::string>::iterator it2 = error_page.find(it->first);
    if (it2 != error_page.end()) {
      s = it2->second;
    }
    AssertEqual(it->second, it2->second);
  }
}

void ConfigTest::defaultConfig() {
  ConfigParser cp;
  Config result = cp.parse("./test2.conf");

  //http
  HttpConfig hc = result.getHttpConfig()[0];
  AssertEqual(8192, hc.getClientBodySize());
  AssertEqual(std::string("/html"), hc.getRoot());

  //server
  ServerConfig sc = result.getHttpConfig()[0].getServerConfig()[0];
  AssertEqual(std::string("127.0.0.1"), sc.getHost());
  AssertEqual(8192, sc.getClientBodySize());
  AssertEqual(std::string("/html"), sc.getRoot());

  // location
  LocationConfig lc = result.getHttpConfig()[0].getServerConfig()[0].getLocationConfig()[0];
  AssertEqual(std::string("/hello"), lc.getPath());
  AssertEqual(false, lc.isAutoIndex());
  AssertEqual(8192, lc.getClientBodySize());
  AssertEqual(std::string("/html"), lc.getRoot());
  AssertEqual(60, lc.getClientBodyTimeout());
  AssertEqual(60, lc.getClientHeaderTimeout());
  AssertEqual(60, lc.getSendTimeout());
}

void ConfigTest::location() {
  ConfigParser cp;
  Config result = cp.parse("./location.conf");

  LocationConfig lc = result.getHttpConfig()[0].getServerConfig()[0].getLocationConfig()[0];

  AssertEqual(std::string("/usr/bin/python3"), lc.getCGI());
  AssertEqual(40, lc.getClientBodyTimeout());
  AssertEqual(41, lc.getClientHeaderTimeout());
  AssertEqual(42, lc.getSendTimeout());
}
