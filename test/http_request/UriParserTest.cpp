#include "./UriParserTest.hpp"

void UriParserTest::testBasic() {
  std::string url = "\
/html/index.html\
?name=kanghyki\
&drink=coffee";

  UriParser qp(url);

  AssertEqual(
      std::string("/html/index.html"),
      qp.getPath());
  std::map<std::string, std::string> q = qp.getQuery();

  AssertEqual((size_t)2, q.size());
  findQueryAndCompare(data(q, "name", "kanghyki"));
  findQueryAndCompare(data(q, "drink", "coffee"));
}

void UriParserTest::testQuestionMarkTwice() {
  std::string url = "\
/html/index.html\
?name=kanghyki?\
&expect_remove?=no";

  UriParser qp(url);

  AssertEqual(
      std::string("/html/index.html"),
      qp.getPath());
  std::map<std::string, std::string> q = qp.getQuery();

  AssertEqual((size_t)2, q.size());
  findQueryAndCompare(data(q, "name", "kanghyki?"));
  findQueryAndCompare(data(q, "expect_remove?", "no"));
}

void UriParserTest::testQuestionMarkAfterEmpty() {
  std::string url = "\
/html/index.html?";

  UriParser qp(url);

  AssertEqual(
      std::string("/html/index.html"),
      qp.getPath());
  std::map<std::string, std::string> q = qp.getQuery();

  AssertEqual((size_t)0, q.size());
}

void UriParserTest::testQuestionMarkAfterOne() {
  std::string url = "\
/html/index.html\
?hello";

  UriParser qp(url);

  AssertEqual(
      std::string("/html/index.html"),
      qp.getPath());
  std::map<std::string, std::string> q = qp.getQuery();

  AssertEqual((size_t)1, q.size());
  findQueryAndCompare(data(q, "hello", ""));
}

void UriParserTest::testEqaulTwice() {
  std::string url = "\
/html/index.html\
?name=kanghyki=apple\
&drink=coffee";

  UriParser qp(url);

  AssertEqual(
      std::string("/html/index.html"),
      qp.getPath());
  std::map<std::string, std::string> q = qp.getQuery();

  AssertEqual((size_t)2, q.size());
  findQueryAndCompare(data(q, "name", "kanghyki"));
  findQueryAndCompare(data(q, "drink", "coffee"));
}

void UriParserTest::testDup() {
  std::string url = "\
/html/index.html\
?name=kanghyki\
&name=0\
&name=1\
&name=2\
&name=3\
&name=apple";

  UriParser qp(url);

  AssertEqual(
      std::string("/html/index.html"),
      qp.getPath());
  std::map<std::string, std::string> q = qp.getQuery();

  AssertEqual((size_t)1, q.size());
  findQueryAndCompare(data(q, "name", "kanghyki"));
}

void UriParserTest::findQueryAndCompare(data data) {
  std::map<std::string, std::string>::const_iterator it = data.q.find(data.key);

  if (it != data.q.end())
    AssertEqual(data.value, it->second);
  else
    AssertEqual(data.value, std::string("undefined"));
}
