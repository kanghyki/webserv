#include "./Token.hpp"

const std::string Token::ILLEGAL                  = "ILLEGAL";
const std::string Token::END_OF_FILE              = "EOF";
const std::string Token::IDENT                    = "IDENT";
const std::string Token::INT                      = "int";
const std::string Token::SEMICOLON                = ";";
const std::string Token::LBRACE                   = "{";
const std::string Token::RBRACE                   = "}";
// Keyword
const std::string Token::HTTP                     = "http";
const std::string Token::SERVER                   = "server";
const std::string Token::LOCATION                 = "location";
const std::string Token::LISTEN                   = "listen";
const std::string Token::SERVER_NAME              = "server_name";
const std::string Token::ROOT                     = "root";
const std::string Token::ERROR_PAGE               = "error_page";
const std::string Token::CLIENT_BODY_BUFFER_SIZE  = "client_body_buffer_size";
const std::string Token::INDEX                    = "index";
const std::string Token::LIMIT_EXCEPT             = "limit_except";
const std::string Token::AUTOINDEX                = "autoindex";
const std::string Token::RETURN                   = "return";
const std::string Token::SESSION_TIMEOUT          = "session_timeout";
const std::string Token::CGI                      = "cgi";
const std::string Token::CLIENT_BODY_TIMEOUT      = "client_body_timeout";
const std::string Token::CLIENT_HEADER_TIMEOUT    = "client_header_timeout";
const std::string Token::SEND_TIMEOUT             = "send_timeout";
const std::string Token::KEEPALIVE_TIMEOUT        = "keepalive_timeout";
const std::string Token::KEEPALIVE_REQUESTS       = "keepalive_requests";

const int         Token::IDENT_IDX                = 0;
const int         Token::TYPE_IDX                 = 1;

const std::string Token::keyword[Token::KEYWORD_SIZE][2] = {
  {"http",                                       Token::HTTP},
  {"server",                                     Token::SERVER},
  {"location",                                   Token::LOCATION},
  {"listen",                                     Token::LISTEN},
  {"server_name",                                Token::SERVER_NAME},
  {"root",                                       Token::ROOT},
  {"error_page",                                 Token::ERROR_PAGE},
  {"client_body_buffer_size",                    Token::CLIENT_BODY_BUFFER_SIZE},
  {"index",                                      Token::INDEX},
  {"limit_except",                               Token::LIMIT_EXCEPT},
  {"autoindex",                                  Token::AUTOINDEX},
  {"return",                                     Token::RETURN},
  {"session_timeout",                            Token::SESSION_TIMEOUT},
  {"cgi",                                        Token::CGI},
  {"client_body_timeout",                        Token::CLIENT_BODY_TIMEOUT},
  {"client_header_timeout",                      Token::CLIENT_HEADER_TIMEOUT},
  {"send_timeout",                               Token::SEND_TIMEOUT},
  {"keepalive_timeout",                          Token::KEEPALIVE_TIMEOUT},
  {"keepalive_requests",                         Token::KEEPALIVE_REQUESTS},
};

Token::Token():
  lineNumber(0),
  pos(0),
  type(ILLEGAL),
  literal("undefined") {}

Token::Token(const Token &obj):
  lineNumber(obj.lineNumber),
  pos(obj.getPos()),
  type(obj.getType()),
  literal(obj.getLiteral()) {}

Token::Token(const std::string& t, const std::string& l):
  lineNumber(0),
  pos(0),
  type(t),
  literal(l) {}

Token::~Token() {}

Token &Token::operator=(const Token& obj) {
  if (this != &obj) {
    this->lineNumber = obj.getLineNumber();
    this->pos = obj.getPos();
    this->type = obj.getType();
    this->literal = obj.getLiteral();
  }
  return *this;
}

bool Token::is(const std::string& type) const {
  if (this->type == type)
    return true;
  return false;
}

bool Token::isNot(const std::string& type) const {
  if (this->type != type)
    return true;
  return false;
}

bool Token::isCommon() const {
  if (is(ROOT) ||
      is(CLIENT_BODY_BUFFER_SIZE) ||
      is(ERROR_PAGE) ||
      is(INDEX))
    return true;
  return false;
}

// getter

std::string Token::getType() const {
  return this->type;
}

std::string Token::getLiteral() const {
  return this->literal;
}

size_t Token::getLineNumber() const {
  return this->lineNumber;
}

size_t Token::getPos() const {
  return this->pos;
}

// setter

void Token::setType(const std::string& type) {
  this->type = type;
}

void Token::setLiteral(const std::string& literal) {
  this->literal = literal;
}

void Token::setLineNumber(size_t lineNumber) {
  this->lineNumber = lineNumber;
}

void Token::setPos(size_t pos) {
  this->pos = pos;
}
