#include <iostream>
#include "./Server.hpp"
#include "./config/parser/ConfigParser.hpp"

const std::string TEAM_MARK = "\
  ███    ██  ██████  ██ ███    ██ ██   ██     ███    ███ ██ ███    ██ ██    ██ ███████\n\
  ████   ██ ██       ██ ████   ██  ██ ██      ████  ████ ██ ████   ██ ██    ██ ██     \n\
  ██ ██  ██ ██   ███ ██ ██ ██  ██   ███       ██ ████ ██ ██ ██ ██  ██ ██    ██ ███████\n\
  ██  ██ ██ ██    ██ ██ ██  ██ ██  ██ ██      ██  ██  ██ ██ ██  ██ ██ ██    ██      ██\n\
  ██   ████  ██████  ██ ██   ████ ██   ██     ██      ██ ██ ██   ████  ██████  ███████\n";

const std::string CONFIG_FILE = "default.conf";

int main(void) {
  std::cout << TEAM_MARK;
  
  try {
    ConfigParser p;
    Config a = p.Parse(CONFIG_FILE);
    Server server(a);
    server.run();
  } catch (std::exception &e) {
    std::cerr << "Error " << e.what() << std::endl;
  }

  return EXIT_SUCCESS;
}
