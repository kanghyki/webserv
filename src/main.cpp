#include "server.hpp"

int main(int argc, char** argv)
{
  if (argc != 2)
    return 1;
  try {
    Server server("", std::atoi(argv[1]));
    server.start();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
	return 0;
}
