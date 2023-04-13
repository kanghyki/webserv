#include "Token.hpp"

Token::Token(const std::string &t, const std::string &l):
  lineNumber(0),
  pos(0),
  type(t),
  literal(l) {}

  // TODO: Remove?
Token::Token():
  lineNumber(-42),
  pos(-42),
  type("undefined"),
  literal("undefined") {}

Token::~Token() {}

Token::Token(const Token &obj):
  lineNumber(obj.lineNumber),
  pos(obj.getPos()),
  type(obj.getType()),
  literal(obj.getLiteral()) {}

Token &Token::operator=(const Token &obj) {
  if (this != &obj) {
    this->lineNumber = obj.getLineNumber();
    this->pos = obj.getPos();
    this->type = obj.getType();
    this->literal = obj.getLiteral();
  }
  return *this;
}

void Token::setLineNumber(size_t lineNumber) {
  this->lineNumber = lineNumber;
}

void Token::setPos(size_t pos) {
  this->pos = pos;
}

size_t Token::getLineNumber() const {
  return this->lineNumber;
}

size_t Token::getPos() const {
  return this->pos;
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

bool Token::is(const std::string &type) const {
  if (this->type == type)
    return true;
  return false;
}

bool Token::isNot(const std::string &type) const {
  if (this->type != type)
    return true;
  return false;
}

bool Token::isCommon() const {
  if (is(token_type::ROOT)
      || is(token_type::CLIENT_BODY_BUFFER_SIZE)
      || is(token_type::ERROR_PAGE)
      || is(token_type::INDEX))
    return true;
  return false;
}
