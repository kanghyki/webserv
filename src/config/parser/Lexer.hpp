#ifndef LEXER_HPP
#define LEXER_HPP

#include "token.hpp"

#include <string>

class Lexer {
  public:
    Lexer();
    Lexer(std::string input);
    ~Lexer();
    Lexer(Lexer const &obj);
    Lexer& operator=(Lexer const &obj);

    void SetInput(std::string input);
    Token NextToken();

  private:
    std::string input;
    int position;
    int read_position;
    char ch;

    void ReadChar();
    char PeekChar();
    std::string ReadIdentifier();
    std::string ReadNumber();
    bool IsDigit(char ch);
    bool IsLetter(char ch);
    bool IsSpace(char ch);
    std::string LookupIdent(std::string ident);
    void SkipWhitespace();

};

#endif
