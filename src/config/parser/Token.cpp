#include "Token.hpp"

Token::Token(const std::string &t, const std::string &l): type(t), literal(l) {}
Token::Token(): type("undefined"), literal("undefined") {}
Token::~Token() {}
Token::Token(const Token &obj): type(obj.getType()), literal(obj.getLiteral()) {}

Token &Token::operator=(const Token &obj) {
  if (this != &obj) {
    this->type = obj.getType();
    this->literal = obj.getLiteral();
  }
  return *this;
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

bool Token::isEqaulType(const std::string &type) {
  if (this->type ==  type)
    return true;
  return false;
}
