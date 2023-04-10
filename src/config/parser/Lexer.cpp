#include "Lexer.hpp"

Lexer::Lexer(std::string input): input(input), \
                                 position(0), \
                                 read_position(0), \
                                 ch(0) {
  ReadChar();
}

Lexer::Lexer(): input(""), position(0), read_position(0), ch(0) {
}

void Lexer::SetInput(std::string input) {
  this->input = input;
  this->position = 0;
  this->read_position = 0;
  this->ch = 0;
  ReadChar();
}

Lexer::Lexer(Lexer const &obj): input(obj.input), \
                                position(obj.position), \
                                read_position(obj.read_position), \
                                ch(obj.ch) {}

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
  if (static_cast<unsigned int>(read_position) >= input.length())
    ch = 0;
  else
    ch = input[read_position];
  position = read_position;
  ++read_position;
}

char Lexer::PeekChar() {
  if (static_cast<unsigned int>(read_position) >= input.length())
    return 0;
  return input[read_position];
}

std::string Lexer::ReadIdentifier() {
  int begin_pos = position;

  while (IsLetter(ch))
    ReadChar();

  return input.substr(begin_pos, position - begin_pos);
}

std::string Lexer::ReadNumber() {
  int begin_pos = position;

  while (IsDigit(ch))
    ReadChar();

  return input.substr(begin_pos, position - begin_pos);
}

bool Lexer::IsDigit(char ch) {
  return std::isdigit(ch);
}

bool Lexer::IsLetter(char ch) {
  if (!IsSpace(ch) &&
      ch != 0 &&
      ch != ';' &&
      ch != '{' &&
      ch != '}') {
    return true;
  }
  return false;
}

bool Lexer::IsSpace(char ch) {
  if (ch == ' ' || ch == '\n' || ch == '\r')
    return true;
  return false;
}

std::string Lexer::LookupIdent(std::string ident) {
  for (int i = 0; i < keyword::SIZE; ++i) {
    if (keywords[i][keyword::IDENT_IDX] == ident)
      return keywords[i][keyword::TYPE_IDX];
  }
  return token_type::IDENT;
}

void Lexer::SkipWhitespace() {
  while (isspace(ch))
    ReadChar();
}

Token Lexer::NextToken() {
  Token ret;

  SkipWhitespace();
  switch (ch) {
    case ';':
      ret = Token(token_type::SEMICOLON, std::string(1, ch));
      break;
    case '{':
      ret = Token(token_type::LBRACE, std::string(1, ch));
      break;
    case '}':
      ret = Token(token_type::RBRACE, std::string(1, ch));
      break;
    case 0:
      ret = Token(token_type::END_OF_FILE, std::string(""));
      break;
    default:
      if (IsLetter(ch)) {
        std::string ident = ReadIdentifier();
        ret = Token(LookupIdent(ident), ident);
        return ret;
      }
      else
        ret = Token(token_type::ILLEGAL, std::string(1, ch));
      break;
  }
  ReadChar();
  return ret;
}
