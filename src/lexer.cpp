/*
* author: kanghyki
* email: kanghyki@gmaicom
* created: 2023-01-23 23:02:24
* updated: 2023-01-27 23:06:38
*/

#include "lexer.hpp"

Lexer::Lexer(): input(""), position(0), read_position(0), ch(0) {
}

Lexer::Lexer(std::string input): input(input), \
                                 position(0), \
                                 read_position(0), \
                                 ch(0) {
  ReadChar();
}

Lexer::Lexer(Lexer const &obj): input(obj.input), \
                                position(obj.position), \
                                read_position(obj.read_position), \
                                ch(obj.ch)
{
}

Lexer &Lexer::operator=(Lexer const &obj) {
  if (this != &obj) {
    input = obj.input;
    position = obj.position;
    read_position = obj.read_position;
    ch = obj.ch;
  }
  return *this;
}

Lexer::~Lexer() {
}

void Lexer::ReadChar() {
  if (static_cast<unsigned int>(read_position) >= input.length()) {
    ch = 0;
  } else {
    ch = input[read_position];
  }
  position = read_position;
  ++read_position;
}

char Lexer::PeekChar() {
  if (static_cast<unsigned int>(read_position) >= input.length()) {
    return 0;
  }
  return input[read_position];
}

std::string Lexer::ReadIdentifier() {
  int begin_pos = position;

  while (IsLetter(ch)) {
    ReadChar();
  }

  return input.substr(begin_pos, position - begin_pos);
}

std::string Lexer::ReadNumber() {
  int begin_pos = position;

  while (IsDigit(ch)) {
    ReadChar();
  }

  return input.substr(begin_pos, position - begin_pos);
}

bool Lexer::IsDigit(char ch) {
  return isdigit(ch);
}

bool Lexer::IsLetter(char ch) {
  if (ch != ';' &&
      ch != '{' &&
      ch != '}' &&
      (isalpha(ch) || ch == '_')) {
    return true;
  }
  return false;
}

std::string Lexer::LookupIdent(std::string ident) {
  for (int i = 0; i < keyword::kSize; ++i) {
    if (keywords[i][keyword::kIdentIdx] == ident) {
      return keywords[i][keyword::kTypeIdx];
    }
  }
  return token_type::kIdent;
}

void Lexer::SkipWhitespace() {
  while (isspace(ch)) {
    ReadChar();
  }
}

Token *Lexer::NextToken() {
  Token *ret;

  SkipWhitespace();
  switch (ch) {
    case ';':
      ret = new Token(token_type::kSemicolon, std::string(1, ch));
      break;
    case '{':
      ret = new Token(token_type::kLbrace, std::string(1, ch));
      break;
    case '}':
      ret = new Token(token_type::kRbrace, std::string(1, ch));
      break;
    case 0:
      ret = new Token(token_type::kEof, std::string(""));
      break;
    default:
      if (IsLetter(ch)) {
        std::string ident = ReadIdentifier();
        ret = new Token(LookupIdent(ident), ident);
        return ret;
      } else if (IsDigit(ch)) {
        ret = new Token(token_type::kInt, ReadNumber());
        return ret;
      } else {
        ret = new Token(token_type::kIllegal, std::string(1, ch));
      }
      break;
  }
  ReadChar();
  return ret;
}
