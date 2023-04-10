#include "Token.hpp"

Token::Token(std::string t, std::string l): type(t), literal(l) {}
Token::Token() {}
Token::~Token() {}
Token::Token(const Token &obj) {}
Token &Token::operator=(const Token &obj) {
  if (this != &obj) {
  }
  return *this;
}

bool Token::operator!=(const Token &obj) {
  if (this->literal != obj.literal ||
      this->type != obj.type)
    return true;
  return false;
}
