#ifndef PARSER_HPP
#define PARSER_HPP

#include "../Config.hpp"
#include "./Token.hpp"
#include "./Lexer.hpp"
#include <fstream>
#include <string>
#include <exception>
#include <vector>

#include <iostream>

class ConfigParser {
  public:
    ConfigParser();
    ~ConfigParser();
    Config Parse(const std::string &fileName);

    class ParseException: public std::exception {
      public:
        const char *what() const throw();
    };

  private:
    std::vector<Token> tokens;
    unsigned long pos;
    std::string fileName;

    void nextToken();
    Token prevToken() const;
    Token curToken() const;
    Token peekToken() const;
    void expectNextToken(const std::string &expected);
    void expectCurToken(const std::string &expected) const;
    void expectError(const std::string &expected) const;
    void badSyntax() const;

    void generateToken(std::string fileName);
    void parseHttp(HttpConfig &conf);
    void parseServer(ServerConfig &conf);
    void parseLocation(LocationConfig &conf);

    void parseCommon(CommonConfig &conf);
    std::vector<std::string> splitStr(std::string line, std::string sep);
};

#endif
