#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include "./Token.hpp"
# include "./Lexer.hpp"
# include "../../etc/Util.hpp"
# include "../Config.hpp"

# include <fstream>
# include <string>
# include <stdexcept>
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
    ServerConfig              parseServer(HttpConfig& httpConf);
    LocationConfig            parseLocation(ServerConfig& conf);
    LocationConfig            parseLocation(LocationConfig& conf);
    void                      parseCommon(CommonConfig& conf);


    // http
    void                      parseClientHeaderTimeout(HttpConfig& conf);
    void                      parseClientBodyTimeout(HttpConfig& conf);
    void                      parseSendTimeout(HttpConfig& conf);

    // server
    void                      parseGatewayTimeout(ServerConfig& conf);
    void                      parseSessionTimeout(ServerConfig& conf);
    void                      parseKeepAliveTimeout(ServerConfig& conf);
    void                      parseKeepAliveRequests(ServerConfig& conf);
    void                      parseListen(ServerConfig& conf);
    void                      parseServerName(ServerConfig& conf);
    void                      parseCGI(ServerConfig& conf);
    // location
    void                      parseAlias(LocationConfig& conf);
    void                      parseLimitExcept(LocationConfig& conf);
    void                      parseAutoindex(LocationConfig& conf);
    void                      parseReturn(LocationConfig& conf);
    // common
    void                      parseRoot(CommonConfig& conf);
    void                      parseErrorPage(CommonConfig& conf);
    void                      parseClientMaxBodySize(CommonConfig& conf);
    void                      parseIndex(CommonConfig& conf);

    void                      generateToken(std::string fileName);

    void                      nextToken();
    Token                     curToken() const;
    Token                     peekToken() const;
    void                      expectNextToken(const std::string& expected);
    void                      expectCurToken(const std::string& expected) const;
    std::string               errorPrefix() const;
    void                      throwError(const std::string& desc) const;
    void                      throwExpectError(const std::string& expected) const;
    void                      throwBadSyntax() const;
    int                       atoi(const std::string& s) const;
};

#endif
