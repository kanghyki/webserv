#include "./Lexer.hpp"

Lexer::Lexer(std::string input):
  input(input),
  position(0),
  read_position(0),
  ch(0) {
    readChar();
}

Lexer::Lexer():
  input(""),
  position(0),
  read_position(0),
  ch(0) {}

void Lexer::setInput(std::string input) {
  this->input = input;
  this->position = 0;
  this->read_position = 0;
  this->ch = 0;
  readChar();
}

Lexer::Lexer(const Lexer& obj):
  input(obj.input),
  position(obj.position),
  read_position(obj.read_position),
  ch(obj.ch) {}

Lexer &Lexer::operator=(const Lexer& obj) {
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
  if (read_position >= input.length())
    ch = 0;
  else
    ch = input[read_position];
  position = read_position;
  ++read_position;
}

char Lexer::peekChar() {
  if (read_position >= input.length())
    return 0;
  return input[read_position];
}

std::string Lexer::readStr() {
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

bool Lexer::isDigit(char ch) const {
  return std::isdigit(ch);
}

// FIXME: 조건 수정
bool Lexer::isLetter(char ch) const {
  if (!isSpace(ch)
      && ch != 0
      && ch != ';'
      && ch != '{'
      && ch != '}')
    return true;
  return false;
}

bool Lexer::isSpace(char ch) const {
  if (ch == ' ' || ch == '\n' || ch == '\r')
    return true;
  return false;
}

bool Lexer::isStrNumber(const std::string &s) const {
  for (int i = 0; i < s.length(); ++i) {
    if (!isDigit(s[i]))
      return false;
  }
  return true;
}

std::string Lexer::lookupIdent(std::string ident) {
  for (int i = 0; i < Token::KEYWORD_SIZE; ++i) {
    if (Token::keyword[i][Token::IDENT_IDX] == ident)
      return Token::keyword[i][Token::TYPE_IDX];
  }
  return Token::IDENT;
}

void Lexer::skipWhitespace() {
  while (std::isspace(ch))
    readChar();
}

Token Lexer::nextToken() {
  Token   ret;
  size_t  beginPosition;

  skipWhitespace();
  beginPosition = getPosition();
  switch (ch) {
    case ';':
      ret = Token(Token::SEMICOLON, std::string(1, ch));
      break;
    case '{':
      ret = Token(Token::LBRACE, std::string(1, ch));
      break;
    case '}':
      ret = Token(Token::RBRACE, std::string(1, ch));
      break;
    case 0:
      ret = Token(Token::END_OF_FILE, std::string(""));
      break;
    default:
      if (isLetter(ch)) {
        std::string word = readStr();
        if (isStrNumber(word))
          ret = Token(Token::INT, word);
        else
          ret = Token(lookupIdent(word), word);
        ret.setPos(beginPosition);
        return ret;
      }
      else
        ret = Token(Token::ILLEGAL, std::string(1, ch));
      break;
  }
  ret.setPos(beginPosition);
  readChar();
  return ret;
}
