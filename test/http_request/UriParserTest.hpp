#ifndef URI_PARSER_TEST_HPP
# define URI_PARSER_TEST_HPP

# include "../test_lib/kunit.hpp"
# include "../../src/http/UriParser.hpp"

# include <map>
# include <string>

struct data {
  data(std::map<std::string, std::string>& q, std::string k, std::string v): q(q), key(k), value(v) {}
  std::map<std::string, std::string>& q;
  std::string key;
  std::string value;
};

class UriParserTest {
  public:
    static void testBasic();
    static void testQuestionMarkTwice();
    static void testQuestionMarkAfterEmpty();
    static void testQuestionMarkAfterOne();
    static void testEqaulTwice();
    static void testDup();
  private:
    static void findQueryAndCompare(struct data data);
};

#endif
