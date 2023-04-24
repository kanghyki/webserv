#ifndef KNUIT_HPP
#define KNUIT_HPP

#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>

class TestError: public std::exception {
  public:
    virtual const char *what() const throw() {
      return "TestError";
    }
};

template<typename T>
void AssertEqual(T a, T b) {
  if (a != b) {
    std::cerr << "Expected: " << a << std::endl;
    std::cerr << "Actual: " << b << std::endl;
  }
  assert(a == b);
}

template<typename T>
void AssertNotEqual(T a, T b) {
  if (a == b) {
    std::cerr << "Expected: " << a << std::endl;
    std::cerr << "Actual: " << b << std::endl;
  }
  assert(a != b);
}

template<typename T>
void AssertSame(T &a, T &b) {
  if (&a != &b) {
    std::cerr << "Expected: Same" << std::endl;
    std::cerr << "Actual: Not same" << std::endl;
  }
  assert(&a == &b);
}

template<typename T>
void AssertNotSame(T &a, T &b) {
  if (&a == &b) {
    std::cerr << "Expected: Not same" << std::endl;
    std::cerr << "Actual: Same" << std::endl;
  }
  assert(&a != &b);
}

void AssertTrue(bool condition);
void AssertFalse(bool condition);
void Fail();

void Run(std::string desc, void (*func)(void));
void SetBeforeEach(void (*func)(void));
void SetAfterEach(void (*func)(void));

namespace test {

  static void (*before)(void) = NULL;
  static void (*after)(void) = NULL;

}

#endif
