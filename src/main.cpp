#include <iostream>
#include "./Server.hpp"
#include "./config/parser/ConfigParser.hpp"
#include "./config/ServerConfig.hpp"
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
    std::cerr << "Error: argument" << std::endl;
    return EXIT_FAILURE;
  }

  try {
    ConfigParser p;
    ServerConfig a = p.parse(argv[1]);
    Log::cout() << INFO << "Config parsing ok\n";
    Server server(a);
    Log::cout() << INFO << "Server setup done\n";
    server.run();
  } catch (std::exception &e) {
    Log::cout() << ERROR << e.what() << "\n";
  }

  return EXIT_SUCCESS;
}
