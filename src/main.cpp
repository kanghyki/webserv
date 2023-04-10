#include <iostream>
#include "config/parser/ConfigParser.hpp"
#include "config/Config.hpp"

int main(void) {
  std::cout << "Hello, Webserv!" << std::endl;
  ConfigParser p;

  try {
    Config a = p.Parse("server.txt");
//    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getHost() << std::endl;
//    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getPort() << std::endl;
//    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getServerName() << std::endl;
//    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getLocationConfig()[0].getPath() << std::endl;

    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getClientBodySize() << std::endl;
    
    a.getHttpConfig()[0].getServerConfig()[0].getLocationConfig()[0].setClientBodySize(42);
    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getLocationConfig()[0].getClientBodySize() << std::endl;
    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getLocationConfig()[0].getClientBodySize() << std::endl;
    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getLocationConfig()[0].getErrorPage().size() << std::endl;
    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getLocationConfig()[0].getIndex()[0] << std::endl;
    std::cout << a.getHttpConfig()[0].getClientBodySize() << std::endl;
  } catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}
