#ifndef CONFIG_TEST_HPP
# define CONFIG_TEST_HPP

# include "../test_lib/kunit.hpp"
# include "../../src/config/parser/ConfigParser.hpp"

class ConfigTest {
  public:
    static void parseTest1();
    static void defaultConfig();
  private:
};

#endif
