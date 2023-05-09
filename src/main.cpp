#include "./main.hpp"


int main(int argc, char **argv) {
  std::string config_file;

  std::cout << TEAM_MARK;

  if (argc < 2) {
    config_file = "default.conf";
    logger::warning << "The argument is not entered" << logger::endl;
  }
  else {
    config_file = argv[1];
    if (argc > 2)
      logger::warning << "Arguments are ignored after one argument" << logger::endl;
  }
  logger::info << "The config file is set to [" << config_file << "]" << logger::endl;

  try {
    ConfigParser parser;
    Config conf = parser.parse(config_file);
    logger::info << "Config parsing ok" << logger::endl;

    Server server(conf);
    logger::info << "Server setup done" << logger::endl;

    server.run();
  } catch (std::exception &e) {
    logger::error << e.what() << logger::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
