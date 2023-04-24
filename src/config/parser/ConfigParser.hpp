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

    Config                    parse(const std::string &fileName);

  private:
    unsigned long             pos;
    std::vector<Token>        tokens;
    std::string               fileName;

    HttpConfig                parseHttp();
    ServerConfig              parseServer(HttpConfig& conf);
    LocationConfig            parseLocation(ServerConfig& conf);
    LocationConfig            parseLocation(LocationConfig& conf);
    LocationConfig            parseLocation();
    void                      parseCommon(CommonConfig& conf);

    // server
    void                      parseListen(ServerConfig& conf);
    void                      parseServerName(ServerConfig& conf);
    // location
    void                      parseAlias(LocationConfig& conf);
    void                      parseLimitExcept(LocationConfig& conf);
    void                      parseAutoIndex(LocationConfig& conf);
    void                      parseReturn(LocationConfig& conf);
    // common
    void                      parseRoot(CommonConfig& conf);
    void                      parseErrorPage(CommonConfig& conf);
    void                      parseClientBodyBufferSize(CommonConfig& conf);
    void                      parseIndex(CommonConfig& conf);

    void                      generateToken(std::string fileName);

    void                      nextToken();
    Token                     curToken() const;
    Token                     peekToken() const;
    void                      expectNextToken(const std::string& expected);
    void                      expectCurToken(const std::string& expected) const;
    void                      expectError(const std::string& expected) const;
    void                      throwBadSyntax() const;
};

#endif
