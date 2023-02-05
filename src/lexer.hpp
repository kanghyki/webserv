/*
* author: kanghyki
* email: kanghyki@gmail.com
* created: 2023-01-23 21:03:19
* updated: 2023-01-27 23:09:19
*/

#ifndef LEXER_HPP
#define LEXER_HPP

#include "token.hpp"

#include <string>

class Lexer {
  public:
    Lexer();
    Lexer(Lexer const &obj);
    Lexer(std::string input);
    Lexer& operator=(Lexer const &obj);
    ~Lexer();

    Token *NextToken();

  private:
    void ReadChar();
    char PeekChar();
    std::string ReadIdentifier();
    std::string ReadNumber();
    bool IsDigit(char ch);
    bool IsLetter(char ch);
    std::string LookupIdent(std::string ident);
    void SkipWhitespace();

  private:
    std::string input;
    int position;
    int read_position;
    char ch;
};

#endif
