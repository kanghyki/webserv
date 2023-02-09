/*
* author: kanghyki
* email: kanghyki@gmail.com
* created: 2023-01-22 15:25:01
* updated: 2023-02-09 18:10:09
*/

#include "lexer.hpp"
#include "file_reader.hpp"
#include "parser.hpp"
#include <iostream>
#include <string>

int main(void) {
  Parser p;

  p.Parse("./default.conf");
//  try {
//  } catch(std::exception ex) {
//    std::cout << "[ what() ]" << std::endl;
//    std::cout << ex.what() << std::endl;
//    std::cout << "[ CurToken() ]" << std::endl;
//    std::cout << p.CurToken()->type << std::endl;
//    std::cout << p.CurToken()->literal << std::endl;
//    std::cout << "[ NextToken() ]" << std::endl;
//    p.NextToken();
//    std::cout << p.CurToken()->type << std::endl;
//    std::cout << p.CurToken()->literal << std::endl;
//  }

  return 0;
}
