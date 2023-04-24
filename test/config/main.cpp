#include <iostream>
#include "../test_lib/kunit.hpp"
#include "./ConfigTest.hpp"

int main(void) {
  Run("test", ConfigTest::ParseTest1);
}
