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

    class RbraceException: public std::exception {
      public:
        const char *what() const throw();
    };
    class ParseException: public std::exception {
      public:
        const char *what() const throw();
    };

  private:
    std::vector<Token> tokens;
    unsigned long pos;
    std::string fileName;

    void nextToken();
    Token prevToken();
    Token curToken();
    Token peekToken();
    void expectTokenType(std::string expected);

    void generateToken(std::string fileName);
    void parseHttp(HttpConfig &conf);
    void parseServer(ServerConfig &conf);
    void parseLocation(LocationConfig &conf);

    void parseCommon(CommonConfig &conf);
    std::vector<std::string> splitStr(std::string line, std::string sep);
};

#endif
