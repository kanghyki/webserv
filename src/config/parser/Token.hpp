#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <string>

class Token {
  public:
    Token();
    Token(const Token& obj);
    Token(const std::string& t, const std::string& l);
    ~Token();
    Token&      operator=(Token const& obj);

    bool        is(const std::string& type) const;
    bool        isNot(const std::string& type) const;
    bool        isCommon() const;

    std::string getType() const;
    std::string getLiteral() const;
    size_t      getLineNumber() const;
    size_t      getPos() const;

    void        setType(const std::string& type);
    void        setLiteral(const std::string& type);
    void        setLineNumber(size_t lineNumber);
    void        setPos(size_t pos);

  private:
    size_t      lineNumber;
    size_t      pos;
    std::string type;
    std::string literal;

};

namespace token_type {
  const std::string ILLEGAL                 = "ILLEGAL";
  const std::string END_OF_FILE             = "EOF";
  const std::string IDENT                   = "IDENT";
  const std::string INT                     = "int";

  const std::string SEMICOLON               = ";";
  const std::string LBRACE                  = "{";
  const std::string RBRACE                  = "}";

  const std::string HTTP                    = "http";
  const std::string SERVER                  = "server";
  const std::string LOCATION                = "location";

  const std::string LISTEN                  = "listen";
  const std::string SERVER_NAME             = "server_name";
  const std::string ROOT                    = "root";
  const std::string ERROR_PAGE              = "error_page";
  const std::string CLIENT_BODY_BUFFER_SIZE = "client_body_buffer_size";
  const std::string INDEX                   = "index";
  const std::string ALIAS                   = "alias";
  const std::string LIMIT_EXCEPT            = "limit_except";
  const std::string AUTOINDEX               = "autoindex";
  const std::string RETURN                  = "return";
};

namespace keyword {
  const int SIZE = 13;
  const int IDENT_IDX = 0;
  const int TYPE_IDX = 1;
};

const std::string keywords[keyword::SIZE][2] = {
  {"http",                     token_type::HTTP},
  {"server",                   token_type::SERVER},
  {"location",                 token_type::LOCATION},
  {"listen",                   token_type::LISTEN},
  {"server_name",              token_type::SERVER_NAME},
  {"root",                     token_type::ROOT},
  {"error_page",               token_type::ERROR_PAGE},
  {"client_body_buffer_size",  token_type::CLIENT_BODY_BUFFER_SIZE},
  {"index",                    token_type::INDEX},
  {"alias",                    token_type::ALIAS},
  {"limit_except",             token_type::LIMIT_EXCEPT},
  {"autoindex",                token_type::AUTOINDEX},
  {"return",                   token_type::RETURN},
};

#endif
