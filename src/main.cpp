#include <iostream>
#include "./Server.hpp"
#include "./config/parser/ConfigParser.hpp"

const std::string TEAM_MARK = "\
  ███    ██  ██████  ██ ███    ██ ██   ██     ███    ███ ██ ███    ██ ██    ██ ███████\n\
  ████   ██ ██       ██ ████   ██  ██ ██      ████  ████ ██ ████   ██ ██    ██ ██     \n\
  ██ ██  ██ ██   ███ ██ ██ ██  ██   ███       ██ ████ ██ ██ ██ ██  ██ ██    ██ ███████\n\
  ██  ██ ██ ██    ██ ██ ██  ██ ██  ██ ██      ██  ██  ██ ██ ██  ██ ██ ██    ██      ██\n\
  ██   ████  ██████  ██ ██   ████ ██   ██     ██      ██ ██ ██   ████  ██████  ███████\n";

int main(int argc, char **argv) {
  std::cout << TEAM_MARK;
  if (argc != 2) {
    std::cerr << "Error: argument" << std::endl;
    return EXIT_FAILURE;
  }

  try {
    ConfigParser p;
    Config a = p.parse(argv[1]);
    Server server(a);
    server.run();
  } catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return EXIT_SUCCESS;
}
