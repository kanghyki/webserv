#ifndef PARSER_HPP
#define PARSER_HPP

#include "../Config.hpp"
#include "./Lexer.hpp"
#include "./Token.hpp"
#include <fstream>
#include <string>
#include <exception>
#include <vector>

class Parser {
  public:
    Parser();
    ~Parser();
    Config Parse(const std::string fileName);

    class RbraceException: public std::exception {
      public:
        const char *what() const throw();
    };
    class ParseException: public std::exception {
      public:
        const char *what() const throw();
    };

  private:
    Config conf;
    std::vector<Token> tokens;
    unsigned long pos;

    void NextToken();
    Token CurToken();
    Token PeekToken();
    void expectTokenType(std::string expected);

    void generateToken(std::string fileName);
    void ParseHttp(HttpConfig conf);
    void ParseServer(ServerConfig conf);
    void ParseLocation(LocationConfig conf);

    void ParseCommon(CommonConfig conf);
    std::vector<std::string> Split(std::string line, std::string sep);
};

#endif
