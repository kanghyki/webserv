#ifndef LEXER_HPP
#define LEXER_HPP

#include "Token.hpp"

#include <string>

class Lexer {
  public:
    Lexer();
    Lexer(std::string input);
    ~Lexer();
    Lexer(Lexer const &obj);
    Lexer& operator=(Lexer const &obj);

    void setInput(std::string input);
    Token nextToken();

    int getPosition() const;

  private:
    std::string input;
    int position;
    int read_position;
    char ch;

    void readChar();
    char peekChar();
    std::string readStr();
    std::string readNumber();
    bool isDigit(char ch) const;
    bool isLetter(char ch) const;
    bool isSpace(char ch) const;
    bool isStrNumber(const std::string &s) const;
    std::string lookupIdent(std::string ident);
    void skipWhitespace();

};

#endif
