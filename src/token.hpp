/*
* author: kanghyki
* email: kanghyki@gmail.com
* created: 2023-01-23 20:47:37
* updated: 2023-01-27 23:10:28
*/

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <unordered_map>
#include <string>

namespace token_type {
  const std::string kIllegal = "ILLEGAL";
  const std::string kEof = "EOF";
  const std::string kIdent = "IDENT";
  const std::string kInt = "INT";

  const std::string kSemicolon = ";";
  const std::string kLbrace = "{";
  const std::string kRbrace = "}";

  const std::string kHttp = "HTTP";
  const std::string kServer = "SERVER";
  const std::string kLocation = "LOCATION";
};

namespace keyword {
  static const int kSize = 3;
  static const int kIdentIdx = 0;
  static const int kTypeIdx = 1;
};
static std::string keywords[keyword::kSize][2] = {
  {"http", token_type::kHttp},
  {"server", token_type::kServer},
  {"location", token_type::kLocation},
};

class Token {
  public:
    Token(std::string t, std::string l): type(t), literal(l) {};
    ~Token() {};

  private:
    Token();
    Token(Token const &obj);
    Token& operator=(Token const &obj);

  public:
    std::string type;
    std::string literal;
};

#endif
