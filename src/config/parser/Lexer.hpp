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
    std::string readIdentifier();
    std::string readNumber();
    bool isDigit(char ch);
    bool isLetter(char ch);
    bool isSpace(char ch);
    std::string lookupIdent(std::string ident);
    void skipWhitespace();

};

#endif
