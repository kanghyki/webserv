#include <iostream>
#include "../test_lib/kunit.hpp"
#include "./HttpRequestTest.hpp"

int main(void) {
  Run("test", HttpRequestTest::test);
  Run("test1", HttpRequestTest::test1);
}
