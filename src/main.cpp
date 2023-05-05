#include "./main.hpp"


int main(int argc, char **argv) {
  std::cout << TEAM_MARK;

  if (argc != 2) {
    log::error << "Required argument. Usage: \"" << argv[0] << " [config_file]\"" << log::endl;
    return EXIT_FAILURE;
  }

  try {
    ConfigParser parser;
    Config conf = parser.parse(argv[1]);
    log::info << "Config parsing ok" << log::endl;
    Server server(conf);
    log::info << "Server setup done" << log::endl;
    server.run();
  } catch (std::exception &e) {
    log::error << e.what() << log::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
