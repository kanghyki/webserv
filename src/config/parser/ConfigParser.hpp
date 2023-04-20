#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include "./Token.hpp"
# include "./Lexer.hpp"
# include "../Config.hpp"
# include "../../Util.hpp"

# include <fstream>
# include <string>
# include <exception>
# include <vector>

class ConfigParser {
  public:
    ConfigParser();
    ~ConfigParser();

    Config                    Parse(const std::string &fileName);

    // TODO: REMOVE
    class ParseException: public std::exception {
      public:
        const char *what() const throw();
    };

  private:
    std::vector<Token>        tokens;
    unsigned long             pos;
    std::string               fileName;

    void                      nextToken();
    Token                     curToken() const;
    Token                     peekToken() const;
    void                      expectNextToken(const std::string& expected);
    void                      expectCurToken(const std::string& expected) const;
    void                      expectError(const std::string& expected) const;
    void                      badSyntax() const;

    void                      generateToken(std::string fileName);
    void                      parseHttp(HttpConfig& conf);
    void                      parseServer(ServerConfig& conf);
    void                      parseLocation(LocationConfig& conf);

    void                      parseCommon(CommonConfig& conf);
};

#endif
