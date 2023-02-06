#ifndef PARSER_HPP
#define PARSER_HPP

#include "configuration.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include "file_reader.hpp"
#include <string>

class Parser {
  public:
    Configuration *Parse(std::string file_name) {
      Configuration *ret = new Configuration;
      FileReader reader(file_name);
      const std::string prompt = ">> ";
      std::string line;

      while (reader.IsReadable()) {
        line = reader.ReadLine();
        std::cout << "=====================" << std::endl;
        std::cout << line << std::endl;
        std::cout << "=====================" << std::endl;
        Lexer l(line);
        Token *tok;
        while ((tok = l.NextToken())->type != token_type::kEof) {
          PrintToken(tok);
          delete tok;
        }
        delete tok;
      }
      return ret;
    };
  private:
    void PrintToken(Token *tok) {
      std::cout << \
        "{Type:" << tok->type << \
        " Literal:" << tok->literal << \
        "}" << std::endl;
    }

};

#endif
