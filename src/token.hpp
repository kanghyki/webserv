/*
* author: kanghyki
* email: kanghyki@gmail.com
* created: 2023-01-23 20:47:37
* updated: 2023-02-06 18:30:27
*/

#ifndef TOKEN_HPP
#define TOKEN_HPP

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

  const std::string kListen = "LISTEN";
  const std::string kServerName = "SERVERNAME";
  const std::string kRoot = "ROOT";
  const std::string kErrorPage = "ERROR_PAGE";
  const std::string kClientBodyBufferSize = "CBBS";
  const std::string kIndex = "INDEX";
  const std::string kAlias = "ALIAS";
  const std::string kLimitExcept = "LIMIT_EXCEPT";
  const std::string kAutoIndex = "AUTO_INDEX";
  const std::string kReturn = "RETURN";
};

namespace keyword {
  static const int kSize = 13;
  static const int kIdentIdx = 0;
  static const int kTypeIdx = 1;
};
static std::string keywords[keyword::kSize][2] = {
  {"http", token_type::kHttp},
  {"server", token_type::kServer},
  {"location", token_type::kLocation},

  {"listen", token_type::kListen},
  {"server_name", token_type::kServerName},
  {"root", token_type::kRoot},
  {"error_page", token_type::kErrorPage},
  {"client_body_buffer_size", token_type::kClientBodyBufferSize},
  {"index", token_type::kIndex},
  {"alias", token_type::kAlias},
  {"limit_except", token_type::kLimitExcept},
  {"auto_index", token_type::kAutoIndex},
  {"return", token_type::kReturn},
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
