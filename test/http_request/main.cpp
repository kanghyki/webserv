#include <iostream>
#include "../test_lib/kunit.hpp"
#include "./HttpRequestTest.hpp"
#include "./UriParserTest.hpp"

int main(void) {
  Run("test", HttpRequestTest::test);
  Run("Query test", UriParserTest::testBasic);
  Run("Query ? 2 test", UriParserTest::testQuestionMarkTwice);
  Run("Query = 2 test", UriParserTest::testEqaulTwice);
  Run("Query ? after empry test", UriParserTest::testQuestionMarkAfterEmpty);
  Run("Query ? after one test", UriParserTest::testQuestionMarkAfterOne);
  Run("Query Dup", UriParserTest::testDup);
}
