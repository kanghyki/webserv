/*
* author: kanghyki
* email: kanghyki@gmail.com
* created: 2023-01-22 15:25:01
* updated: 2023-02-06 17:20:26
*/

#include "lexer.hpp"
#include "file_reader.hpp"
#include "parser.hpp"
#include <iostream>
#include <string>

int main(void) {
  Parser p;

  p.Parse("./default.conf");
  return 0;
}
