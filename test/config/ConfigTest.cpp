#include "./ConfigTest.hpp"

void ConfigTest::ParseTest1() {
  ConfigParser cp;
  Config result = cp.parse("./test1.conf");
  ServerConfig sc = result.getHttpConfig()[0].getServerConfig()[0];

  short port = sc.getPort();
  std::string host = sc.getHost();
  std::string server_name = sc.getServerName();
  std::vector<std::string> index = sc.getIndex();
  int client_body_buffer_size = sc.getClientBodySize();
  std::string root = sc.getRoot();
  std::string error_page_path = sc.getErrorPagePath();
  std::vector<int> error_page_status = sc.getErrorPageStatus();

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
  AssertEqual(error_page_path, std::string("/html/error.html"));
  std::vector<int> expect2;
  expect2.push_back(404);
  for (auto it = error_page_status.begin(), it2 = expect2.begin(); it != error_page_status.end(); ++it, ++it2) {
    AssertEqual((*it), (*it2));
  }
}
