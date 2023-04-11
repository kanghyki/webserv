#include <iostream>
#include "Server.hpp"
#include "config/parser/ConfigParser.hpp"
#include "config/Config.hpp"

const std::string TEAM_MARK = "\
  ███    ██  ██████  ██ ███    ██ ██   ██     ███    ███ ██ ███    ██ ██    ██ ███████\n\
  ████   ██ ██       ██ ████   ██  ██ ██      ████  ████ ██ ████   ██ ██    ██ ██     \n\
  ██ ██  ██ ██   ███ ██ ██ ██  ██   ███       ██ ████ ██ ██ ██ ██  ██ ██    ██ ███████\n\
  ██  ██ ██ ██    ██ ██ ██  ██ ██  ██ ██      ██  ██  ██ ██ ██  ██ ██ ██    ██      ██\n\
  ██   ████  ██████  ██ ██   ████ ██   ██     ██      ██ ██ ██   ████  ██████  ███████\n";

int main(int argc, char** argv) {
  std::cout << TEAM_MARK;
  

  if (argc != 2) {
    std::cerr << "Error: Invalid number of arguments!" << std::endl;
    return EXIT_FAILURE;
  }
  
  
  // ------------------------- kanghyki -------------------------
  try {
    ConfigParser p;
    Config a = p.Parse("default.conf");
//    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getHost() << std::endl;
//    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getPort() << std::endl;
//    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getServerName() << std::endl;
//    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getLocationConfig()[0].getPath() << std::endl;

    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getClientBodySize() << std::endl;
    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getLocationConfig()[0].getClientBodySize() << std::endl;
    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getRoot() << std::endl;
    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getIndex()[0] << std::endl;
    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getIndex()[1] << std::endl;
    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getIndex()[2] << std::endl;
    std::cout << a.getHttpConfig()[0].getServerConfig()[0].getLocationConfig()[0].getErrorPage().size() << std::endl;
    std::cout << a.getHttpConfig()[0].getClientBodySize() << std::endl;
  } catch (std::exception &e) {
    std::cerr << "Error|" << e.what() << std::endl;
  }

  // ------------------------- hyeongki -------------------------
//  try {
//    Server server;
//    server.socketInit("", std::atoi(argv[1]));
//    server.run();
//  } catch (std::exception& e) {
//    std::cerr << "Error: " << e.what() << std::endl;
//  }

  return EXIT_SUCCESS;
}
