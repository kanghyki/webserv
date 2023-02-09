#include "server.hpp"
#include "lexer.hpp"
#include "file_reader.hpp"
#include "parser.hpp"
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  if (argc != 2)
    return 1;

  Parser p;
  p.Parse("./default.conf");

  try {
    Server server("", std::atoi(argv[1]));
    server.Start();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}
