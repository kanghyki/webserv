#include "Token.hpp"

Token::Token(const std::string &t, const std::string &l): 
  line(0), word(0), type(t), literal(l) {}
Token::Token():
  line(0), word(0), type("undefined"), literal("undefined") {}
Token::~Token() {}
Token::Token(const Token &obj):
  line(obj.line), word(obj.word), type(obj.getType()), literal(obj.getLiteral()) {}

Token &Token::operator=(const Token &obj) {
  if (this != &obj) {
    this->type = obj.getType();
    this->literal = obj.getLiteral();
  }
  return *this;
}

void Token::setLine(size_t line) {
  this->line = line;
}

void Token::setWord(size_t word) {
  this->word = word;
}

size_t Token::getLine() const {
  return this->line;
}

size_t Token::getWord() const {
  return this->word;
}

std::string Token::getType() const {
  return this->type;
}

std::string Token::getLiteral() const {
  return this->literal;
}

void Token::setType(const std::string &type) {
  this->type = type;
}

void Token::setLiteral(const std::string &literal) {
  this->literal = literal;
}

bool Token::isType(const std::string &type) const {
  if (this->type == type)
    return true;
  return false;
}

bool Token::isNotType(const std::string &type) const {
  if (this->type != type)
    return true;
  return false;
}

bool Token::isCommon() const {
  if (isType(token_type::ROOT)
      || isType(token_type::CLIENT_BODY_BUFFER_SIZE)
      || isType(token_type::ERROR_PAGE)
      || isType(token_type::INDEX))
    return true;
  return false;
}

bool Token::isListen() const {
  if (isType(token_type::LISTEN))
    return true;
  return false;
}

bool Token::isServerName() const {
  if (isType(token_type::SERVER_NAME))
    return true;
  return false;
}

bool Token::isLocation() const {
  if (isType(token_type::LOCATION))
    return true;
  return false;
}

bool Token::isServer() const {
  if (isType(token_type::SERVER))
    return true;
  return false;
}

bool Token::isHttp() const {
  if (isType(token_type::HTTP))
    return true;
  return false;
}

bool Token::isAlias() const {
  if (isType(token_type::ALIAS))
    return true;
  return false;
}

bool Token::isLimitExcept() const {
  if (isType(token_type::LIMIT_EXCEPT))
    return true;
  return false;
}

bool Token::isAutoIndex() const {
  if (isType(token_type::AUTO_INDEX))
    return true;
  return false;
}

bool Token::isReturn() const {
  if (isType(token_type::RETURN))
    return true;
  return false;
}

bool Token::isRoot() const {
  if (isType(token_type::ROOT))
    return true;
  return false;
}

bool Token::isErrorPage() const {
  if (isType(token_type::ERROR_PAGE))
    return true;
  return false;
}

bool Token::isClientBodyBufferSize() const {
  if (isType(token_type::CLIENT_BODY_BUFFER_SIZE))
    return true;
  return false;
}

bool Token::isIndex() const {
  if (isType(token_type::INDEX))
    return true;
  return false;
}
