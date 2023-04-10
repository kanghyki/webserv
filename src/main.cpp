#include "Server.hpp"

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

  try {
    Server server;
    server.socketInit("", std::atoi(argv[1]));
    server.run();
  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return EXIT_SUCCESS;
}
