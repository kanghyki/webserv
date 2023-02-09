#ifndef PARSER_HPP
#define PARSER_HPP

#include "configuration.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include "file_reader.hpp"
#include <string>
#include <exception>
#include <vector>

class Parser {
  public:
    Parser();
    ~Parser();
    Configuration *Parse(std::string file_name);

    class LbraceException: public std::exception {
      public:
        const char *what() const throw();
    };
    class RbraceException: public std::exception {
      public:
        const char *what() const throw();
    };
    class SemicolonException: public std::exception {
      public:
        const char *what() const throw();
    };
    class RootException: public std::exception {
      public:
        const char *what() const throw();
    };
    class ErrorPageException: public std::exception {
      public:
        const char *what() const throw();
    };
    class CBBSException: public std::exception {
      public:
        const char *what() const throw();
    };
    class IndexException: public std::exception {
      public:
        const char *what() const throw();
    };
    class ParseException: public std::exception {
      public:
        const char *what() const throw();
    };

    void NextToken();
    Token *CurToken();
    Token *PeekToken();
  private:
    Configuration *conf;
    std::vector<Token *> t_arr;
    unsigned long pos;

    void ReadAllToken(std::string file_name);
    void PrintToken(Token *tok);
    void ParseHttp(HttpConfiguration *conf);
    void ParseServer(ServerConfiguration *conf);
    void ParseLocation(LocationConfiguration *conf);

    void ParseCommon(AConfiguration *conf);
    std::vector<std::string> Split(std::string line, std::string sep);
};

#endif
