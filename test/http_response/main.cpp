#include <iostream>
#include "../test_lib/kunit.hpp"
#include "./HttpResponseTest.hpp"

int main(void) {
  Run("response toString test", HttpResponseTest::response_toString_test);
  Run("response toString test2", HttpResponseTest::response_toString_test2);
}
