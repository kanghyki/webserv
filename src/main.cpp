#include <iostream>
#include "./Server.hpp"
#include "./config/parser/ConfigParser.hpp"
#include "./config/ServerConfig.hpp"
#include "./Log.hpp"

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

  Log::cout() << DEBUG << "hello";
  Log::cout() << WARNING << "hello";
  Log::cout() << ERROR << "hello";
  Log::cout() << DEBUG << "hello";

  try {
    ConfigParser p;
    ServerConfig a = p.parse(argv[1]);
    Server server(a);
    server.run();
  } catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return EXIT_SUCCESS;
}
