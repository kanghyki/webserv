#ifndef LEXER_HPP
# define LEXER_HPP

# include "./Token.hpp"

# include <string>

class Lexer {
  public:
    Lexer();
    Lexer(const Lexer& obj);
    Lexer(std::string input);
    ~Lexer();
    Lexer&        operator=(const Lexer& obj);

    void          setInput(std::string input);
    Token         nextToken();

    int           getPosition() const;

  private:
    std::string   input;
    unsigned int  position;
    unsigned int  read_position;
    char          ch;

    void          readChar();
    char          peekChar();
    std::string   readWord();
    bool          isWord(char ch) const;
    bool          isWordNumber(const std::string &s) const;
    std::string   lookupIdent(std::string ident);
    void          skipWhitespace();

};

#endif
