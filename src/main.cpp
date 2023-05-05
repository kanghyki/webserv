#include "./main.hpp"


int main(int argc, char **argv) {
  std::string config_file;

  std::cout << TEAM_MARK;

  if (argc < 2) {
    config_file = "default.conf";
    log::warning << "The argument is not entered" << log::endl;
  }
  else {
    config_file = argv[1];
    if (argc > 2)
      log::warning << "Arguments are ignored after one argument" << log::endl;
  }
  log::info << "The config file is set to [" << config_file << "]" << log::endl;

  try {
    ConfigParser parser;
    Config conf = parser.parse(config_file);
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
