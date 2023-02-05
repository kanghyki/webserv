/*
* author: kanghyki
* email: kanghyki@gmail.com
* created: 2023-01-22 15:25:01
* updated: 2023-01-27 23:06:41
*/

#include "lexer.hpp"
#include <iostream>
#include <string>

void PrintToken(Token *tok) {
  std::cout << \
    "{Type:" << tok->type << \
    " Literal:" << tok->literal << \
    "}" << std::endl;
}

int main(void) {
  const std::string prompt = ">> ";
  std::string input;

  while (true) {
    std::cout << prompt;
    if (std::getline(std::cin, input).eof() == true) {
      exit(0);
    }
    Lexer l(input);
    Token *tok;
    while ((tok = l.NextToken())->type != token_type::kEof) {
      PrintToken(tok);
      delete tok;
    }
    delete tok;
	}
	return 0;
}
