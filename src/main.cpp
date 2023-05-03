#include <iostream>
#include "./Server.hpp"
#include "./config/parser/ConfigParser.hpp"
#include "./config/Config.hpp"
#include "./Logger.hpp"

const std::string TEAM_MARK = "\
  ███    ██  ██████  ██ ███    ██ ██   ██     ███    ███ ██ ███    ██ ██    ██ ███████\n\
  ████   ██ ██       ██ ████   ██  ██ ██      ████  ████ ██ ████   ██ ██    ██ ██     \n\
  ██ ██  ██ ██   ███ ██ ██ ██  ██   ███       ██ ████ ██ ██ ██ ██  ██ ██    ██ ███████\n\
  ██  ██ ██ ██    ██ ██ ██  ██ ██  ██ ██      ██  ██  ██ ██ ██  ██ ██ ██    ██      ██\n\
  ██   ████  ██████  ██ ██   ████ ██   ██     ██      ██ ██ ██   ████  ██████  ███████\n";

int main(int argc, char **argv) {
  std::cout << TEAM_MARK;
  if (argc != 2) {
    log::error << "argument";
    return EXIT_FAILURE;
  }

  try {
    ConfigParser p;
    Config a = p.parse(argv[1]);
    log::info << "Config parsing ok" << log::endl;
    Server server(a);
    log::info << "Server setup done" << log::endl;
    server.run();
  } catch (std::exception &e) {
    log::error << e.what() << log::endl;
  }

  return EXIT_SUCCESS;
}
