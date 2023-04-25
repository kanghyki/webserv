#include <iostream>
#include "../test_lib/kunit.hpp"
#include "./ConfigTest.hpp"

int main(void) {
  Run("test1", ConfigTest::parseTest1);
  Run("test2", ConfigTest::defaultConfig);
}
