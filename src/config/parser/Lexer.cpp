#include "Lexer.hpp"

Lexer::Lexer(std::string input): input(input), \
                                 position(0), \
                                 read_position(0), \
                                 ch(0) {
  readChar();
}

Lexer::Lexer(): input(""), position(0), read_position(0), ch(0) {
}

void Lexer::setInput(std::string input) {
  this->input = input;
  this->position = 0;
  this->read_position = 0;
  this->ch = 0;
  readChar();
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

int Lexer::getPosition() const {
  return this->position;
}

void Lexer::readChar() {
  if (static_cast<unsigned int>(read_position) >= input.length())
    ch = 0;
  else
    ch = input[read_position];
  position = read_position;
  ++read_position;
}

char Lexer::peekChar() {
  if (static_cast<unsigned int>(read_position) >= input.length())
    return 0;
  return input[read_position];
}

std::string Lexer::readIdentifier() {
  int begin_pos = position;

  while (isLetter(ch))
    readChar();

  return input.substr(begin_pos, position - begin_pos);
}

std::string Lexer::readNumber() {
  int begin_pos = position;

  while (isDigit(ch))
    readChar();

  return input.substr(begin_pos, position - begin_pos);
}

bool Lexer::isDigit(char ch) {
  return std::isdigit(ch);
}

bool Lexer::isLetter(char ch) {
  if (!isSpace(ch) &&
      ch != 0 &&
      ch != ';' &&
      ch != '{' &&
      ch != '}') {
    return true;
  }
  return false;
}

bool Lexer::isSpace(char ch) {
  if (ch == ' ' || ch == '\n' || ch == '\r')
    return true;
  return false;
}

std::string Lexer::lookupIdent(std::string ident) {
  for (int i = 0; i < keyword::SIZE; ++i) {
    if (keywords[i][keyword::IDENT_IDX] == ident)
      return keywords[i][keyword::TYPE_IDX];
  }
  return token_type::IDENT;
}

void Lexer::skipWhitespace() {
  while (isspace(ch))
    readChar();
}

Token Lexer::nextToken() {
  Token ret;

  skipWhitespace();
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
      if (isLetter(ch)) {
        std::string ident = readIdentifier();
        ret = Token(lookupIdent(ident), ident);
        return ret;
      }
      else
        ret = Token(token_type::ILLEGAL, std::string(1, ch));
      break;
  }
  readChar();
  return ret;
}
