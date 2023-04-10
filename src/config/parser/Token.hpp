#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

namespace token_type {
  const std::string ILLEGAL = "ILLEGAL";
  const std::string END_OF_FILE = "EOF";
  const std::string IDENT = "IDENT";
  const std::string INT = "INT";

  const std::string SEMICOLON = ";";
  const std::string LBRACE = "{";
  const std::string RBRACE = "}";

  const std::string HTTP = "HTTP";
  const std::string SERVER = "SERVER";
  const std::string LOCATION = "LOCATION";

  const std::string LISTEN = "LISTEN";
  const std::string SERVER_NAME = "SERVERNAME";
  const std::string ROOT = "ROOT";
  const std::string ERROR_PAGE = "ERROR_PAGE";
  const std::string CLIENT_BODY_BUFFER_SIZE = "CBBS";
  const std::string INDEX = "INDEX";
  const std::string ALIAS = "ALIAS";
  const std::string LIMIT_EXCEPT = "LIMIT_EXCEPT";
  const std::string AUTO_INDEX = "AUTO_INDEX";
  const std::string RETURN = "RETURN";
};

namespace keyword {
  const int SIZE = 13;
  const int IDENT_IDX = 0;
  const int TYPE_IDX = 1;
};

const std::string keywords[keyword::SIZE][2] = {
  {"http", token_type::HTTP},
  {"server", token_type::SERVER},
  {"location", token_type::LOCATION},
  {"listen", token_type::LISTEN},
  {"server_name", token_type::SERVER_NAME},
  {"root", token_type::ROOT},
  {"error_page", token_type::ERROR_PAGE},
  {"client_body_buffer_size", token_type::CLIENT_BODY_BUFFER_SIZE},
  {"index", token_type::INDEX},
  {"alias", token_type::ALIAS},
  {"limit_except", token_type::LIMIT_EXCEPT},
  {"auto_index", token_type::AUTO_INDEX},
  {"return", token_type::RETURN},
};

class Token {
  public:
    Token(std::string t, std::string l);
    Token();
    Token(Token const &obj);
    Token& operator=(Token const &obj);
    ~Token();
    bool operator!=(Token const &obj);

  private:

  public:
    std::string type;
    std::string literal;
};

#endif
