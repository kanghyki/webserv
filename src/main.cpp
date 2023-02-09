#include "server.hpp"

int main(int argc, char** argv) {
  Parser p;
  if (argc != 2)
    return 1;

  p.Parse("./default.conf");
  try {
    Server server("", std::atoi(argv[1]));
    server.Start();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
