#include "./kunit.hpp"

void Run(std::string desc, void (*func)(void)) {
  static int count = 0;
  pid_t pid;
  int status = 0;
  clock_t s, e;

  std::cout << "\n\"" << desc << "\"" << std::endl;
  s = clock();
  pid = fork();
  if (pid < 0) {
    throw TestError();
  } else if (pid == 0) {
    if (test::before) test::before();
    func();
    if (test::after) test::after();
    exit(EXIT_SUCCESS);
  }
  waitpid(pid, &status, 0);
  e = clock();
  std::cout << "[" << count << "]" << " Elapsed time: " << e - s << "ms\n\n";
  ++count;
}

void SetBeforeEach(void (*func)(void)) {
  test::before = func;
}

void SetAfterEach(void (*func)(void)) {
  test::after = func;
}

void AssertTrue(bool condition) {
  if (condition == false) {
    std::cerr << "Expected: true" << std::endl;
    std::cerr << "Actual: false" << std::endl;
  }
  assert(condition == true);
}

void AssertFalse(bool condition) {
  if (condition == false) {
    std::cerr << "Expected: false" << std::endl;
    std::cerr << "Actual: true" << std::endl;
  }
  assert(condition == false);
}

void Fail() {
  std::cerr  << "[ forced failure ]" << std::endl;
  assert(false != true);
}
