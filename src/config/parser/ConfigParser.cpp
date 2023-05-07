#include "./ConfigParser.hpp"

ConfigParser::ConfigParser() {
  pos = 0;
};

ConfigParser::~ConfigParser() {};

Config ConfigParser::parse(const std::string& fileName) throw (std::runtime_error) {
  Config conf;

  generateToken(fileName);
  while (curToken().isNot(Token::END_OF_FILE)) {
    if (curToken().is(Token::HTTP)) conf.setHttpConfig(parseHttp());
    else throwBadSyntax();
    nextToken();
  }
  expectCurToken(Token::END_OF_FILE);

  if (conf.getHttpConfig().getServerConfig().size() == 0) throw (std::runtime_error("No server config found"));

  return conf;
}

HttpConfig ConfigParser::parseHttp() {
  HttpConfig conf;

  expectNextToken(Token::LBRACE);
  for (nextToken(); curToken().isNot(Token::END_OF_FILE) && curToken().isNot(Token::RBRACE); nextToken()) {
    if (curToken().is(Token::SERVER)) conf.addServerConfig(parseServer(conf));
    else if (curToken().is(Token::CLIENT_HEADER_TIMEOUT)) parseClientHeaderTimeout(conf);
    else if (curToken().is(Token::CLIENT_BODY_TIMEOUT)) parseClientBodyTimeout(conf);
    else if (curToken().is(Token::SEND_TIMEOUT)) parseSendTimeout(conf);
    else if (curToken().isCommon()) parseCommon(conf);
    else throwBadSyntax();
  }
  expectCurToken(Token::RBRACE);

  return conf;
}

ServerConfig ConfigParser::parseServer(HttpConfig& httpConf) {
  ServerConfig conf(httpConf);

  expectNextToken(Token::LBRACE);
  for (nextToken(); curToken().isNot(Token::END_OF_FILE) && curToken().isNot(Token::RBRACE); nextToken()) {
    if (curToken().is(Token::LOCATION)) conf.addLocationConfig(parseLocation(conf));
    else if (curToken().isCommon()) parseCommon(conf);
    else if (curToken().is(Token::SESSION_TIMEOUT)) parseSessionTimeout(conf);
    else if (curToken().is(Token::KEEPALIVE_TIMEOUT)) parseKeepAliveTimeout(conf);
    else if (curToken().is(Token::KEEPALIVE_REQUESTS)) parseKeepAliveRequests(conf);
    else if (curToken().is(Token::LISTEN)) parseListen(conf);
    else if (curToken().is(Token::SERVER_NAME)) parseServerName(conf);
    else if (curToken().is(Token::CGI)) parseCGI(conf);
    else throwBadSyntax();
  }
  expectCurToken(Token::RBRACE);

  return conf;
}

LocationConfig ConfigParser::parseLocation(ServerConfig& serverConf) {
  LocationConfig conf(serverConf);

  expectNextToken(Token::IDENT);
  conf.setPath(curToken().getLiteral());
  expectNextToken(Token::LBRACE);
  for (nextToken(); curToken().isNot(Token::END_OF_FILE) && curToken().isNot(Token::RBRACE); nextToken()) {
    if (curToken().is(Token::LOCATION)) conf.addLocationConfig(parseLocation(conf));
    else if (curToken().isCommon()) parseCommon(conf);
    else if (curToken().is(Token::LIMIT_EXCEPT)) parseLimitExcept(conf);
    else if (curToken().is(Token::AUTOINDEX)) parseAutoindex(conf);
    else if (curToken().is(Token::RETURN)) parseReturn(conf);
    else throwBadSyntax();
  }
  expectCurToken(Token::RBRACE);

  return conf;
}

LocationConfig ConfigParser::parseLocation(LocationConfig& locationConf) {
  LocationConfig conf(locationConf);

  expectNextToken(Token::IDENT);
  conf.setPath(curToken().getLiteral());
  expectNextToken(Token::LBRACE);
  for (nextToken(); curToken().isNot(Token::END_OF_FILE) && curToken().isNot(Token::RBRACE); nextToken()) {
    if (curToken().is(Token::LOCATION)) conf.addLocationConfig(parseLocation(conf));
    else if (curToken().isCommon()) parseCommon(conf);
    else if (curToken().is(Token::LIMIT_EXCEPT)) parseLimitExcept(conf);
    else if (curToken().is(Token::AUTOINDEX)) parseAutoindex(conf);
    else if (curToken().is(Token::RETURN)) parseReturn(conf);
    else throwBadSyntax();
  }
  expectCurToken(Token::RBRACE);

  return conf;
}

void ConfigParser::parseCommon(CommonConfig& conf) {
  if (curToken().is(Token::ROOT)) parseRoot(conf);
  else if (curToken().is(Token::ERROR_PAGE)) parseErrorPage(conf);
  else if (curToken().is(Token::CLIENT_MAX_BODY_SIZE)) parseClientMaxBodySize(conf);
  else if (curToken().is(Token::INDEX)) parseIndex(conf);
}

// http
// http
// http

// client_header_timeout [second(int)];
void ConfigParser::parseClientHeaderTimeout(HttpConfig& conf) {
  expectNextToken(Token::INT);
  conf.setClientHeaderTimeout(atoi(curToken().getLiteral()));
  expectNextToken(Token::SEMICOLON);
}

// client_body_timeout [second(int)];
void ConfigParser::parseClientBodyTimeout(HttpConfig& conf) {
  expectNextToken(Token::INT);
  conf.setClientBodyTimeout(atoi(curToken().getLiteral()));
  expectNextToken(Token::SEMICOLON);
}

// send_timeout [second(int)];
void ConfigParser::parseSendTimeout(HttpConfig& conf) {
  expectNextToken(Token::INT);
  conf.setSendTimeout(atoi(curToken().getLiteral()));
  expectNextToken(Token::SEMICOLON);
}

// server
// server
// server

// session_timeout [second(int)];
void ConfigParser::parseSessionTimeout(ServerConfig& conf) {
  expectNextToken(Token::INT);
  conf.setSessionTimeout(atoi(curToken().getLiteral()));
  expectNextToken(Token::SEMICOLON);
}

// keepalive_timeout [second(int)];
void ConfigParser::parseKeepAliveTimeout(ServerConfig& conf) {
  expectNextToken(Token::INT);
  conf.setKeepAliveTimeout(atoi(curToken().getLiteral()));
  expectNextToken(Token::SEMICOLON);
}

// keepalive_requests [second(int)];
void ConfigParser::parseKeepAliveRequests(ServerConfig& conf) {
  expectNextToken(Token::INT);
  conf.setKeepAliveRequests(atoi(curToken().getLiteral()));
  expectNextToken(Token::SEMICOLON);
}

// listen [host(ident)]:[port(int)];
void ConfigParser::parseListen(ServerConfig& conf) {
  expectNextToken(Token::IDENT);
  std::vector<std::string> sp = util::split(curToken().getLiteral(), ':');
  if (sp.size() != 2) throwError("listen error");
  conf.setHost(sp[0]);
  conf.setPort(atoi(sp[1]));
  expectNextToken(Token::SEMICOLON);
}

// server_name [path(ident)];
void ConfigParser::parseServerName(ServerConfig& conf) {
  expectNextToken(Token::IDENT);
  conf.setServerName(curToken().getLiteral());
  expectNextToken(Token::SEMICOLON);
}

// location
// location
// location

// limit_except [HTTP method(indent) ...] ;
void ConfigParser::parseLimitExcept(LocationConfig& conf) {
  while (peekToken().is(Token::IDENT)) {
    nextToken();
    conf.addLimitExcept(curToken().getLiteral());
  }
  expectNextToken(Token::SEMICOLON);
}

// cgi [extension(indent)] [CGI path(ident)]
void ConfigParser::parseCGI(ServerConfig& conf) {
  std::string ext;
  std::string path;

  expectNextToken(Token::IDENT);
  ext = curToken().getLiteral();
  expectNextToken(Token::IDENT);
  path = curToken().getLiteral();
  conf.insertCGI(ext, path);
  expectNextToken(Token::SEMICOLON);
}

// autoindex [on(ident)/off(ident)]
void ConfigParser::parseAutoindex(LocationConfig& conf) {
  expectNextToken(Token::IDENT);
  if (curToken().getLiteral() == "on") conf.setAutoindex(true);
  else if (curToken().getLiteral() == "off") conf.setAutoindex(false);
  else throwError("autoindex error");
  expectNextToken(Token::SEMICOLON);
}

// return [HTTP status code(int)] [URI(ident)]
void ConfigParser::parseReturn(LocationConfig& conf) {
  expectNextToken(Token::INT);
  int status_code = atoi(curToken().getLiteral());
  if (status_code != 301  && status_code != 307)
    throwError("A value other than 301 or 307 is not supported by the return field");
  expectNextToken(Token::IDENT);
  conf.setReturnRes(status_code, curToken().getLiteral());
  expectNextToken(Token::SEMICOLON);
}

// common
// common
// common

// root [path(ident)]
void ConfigParser::parseRoot(CommonConfig& conf) {
  expectNextToken(Token::IDENT);
  conf.setRoot(curToken().getLiteral());
  expectNextToken(Token::SEMICOLON);
}

// error_page [HTTP status code(int) ...] [path(ident)]
void ConfigParser::parseErrorPage(CommonConfig& conf) {
  std::vector<int> statusList;

  while (peekToken().is(Token::INT)) {
    nextToken();
    int status = atoi(curToken().getLiteral());
    statusList.push_back(status);
  }

  expectNextToken(Token::IDENT);

  for (size_t i = 0; i < statusList.size(); ++i)
    conf.addErrorPage(statusList[i], curToken().getLiteral());
  expectNextToken(Token::SEMICOLON);
}

// client_body_buffer_size [size(int)]
void ConfigParser::parseClientMaxBodySize(CommonConfig& conf) {
  expectNextToken(Token::INT);
  conf.setClientMaxBodySize(atoi(curToken().getLiteral()));
  expectNextToken(Token::SEMICOLON);
}

// index [file_name(ident)]
void ConfigParser::parseIndex(CommonConfig& conf) {
  expectNextToken(Token::IDENT);
  conf.setIndex(curToken().getLiteral());
  expectNextToken(Token::SEMICOLON);
}

void ConfigParser::generateToken(std::string fileName) {
  size_t        lineCount = 1;
  Lexer         lexer;
  Token         token;
  std::string   line;
  std::ifstream fileIn(fileName.c_str(), std::ifstream::in);

  this->fileName = fileName;
  if (!fileIn.is_open())
    throw std::runtime_error("config file open failed");
  for (;!std::getline(fileIn, line).eof(); ++lineCount) {
    lexer.setInput(line);
    while ((token = lexer.nextToken()).isNot(Token::END_OF_FILE)) {
      token.setLineNumber(lineCount);
      tokens.push_back(token);
    }
  }
  tokens.push_back(Token(Token::END_OF_FILE, "END_OF_FILE"));
}

void ConfigParser::nextToken() {
  if (this->pos + 1 < tokens.size()) {
    this->pos += 1;
  }
};

Token ConfigParser::curToken() const {
  return this->tokens[this->pos];
};

Token ConfigParser::peekToken() const {
  if (this->pos + 1 >= this->tokens.size())
    throw std::runtime_error("tokenization error");
  return this->tokens[this->pos + 1];
}

void ConfigParser::expectNextToken(const std::string& expected) {
  nextToken();

  if (curToken().getType() != expected)
    throwExpectError(expected);
}

void ConfigParser::expectCurToken(const std::string& expected) const {
  if (curToken().getType() != expected)
    throwExpectError(expected);
}

std::string ConfigParser::errorPrefix() const {
  return this->fileName + " " + std::to_string(curToken().getLineNumber()) + ":" + util::itoa(curToken().getPos()) + " ";
}

void ConfigParser::throwError(const std::string& desc) const throw (std::runtime_error) {
  std::string errorMsg = errorPrefix() + desc;

  throw std::runtime_error(errorMsg);
}

void ConfigParser::throwExpectError(const std::string& expected) const throw (std::runtime_error) {
  throwError("expected \'" + expected + "\' but \'" + curToken().getLiteral() + "\'");
}

void ConfigParser::throwBadSyntax() const throw (std::runtime_error){
  throwError("bad syntax \'" + curToken().getLiteral() + "\'");
}

int ConfigParser::atoi(const std::string& s) const {
  int ret;

  for (size_t i = 0; i < s.length(); ++i) {
    if (std::isdigit(s[i]) == false)
      throwBadSyntax();
  }

  try {
    ret = std::atoi(s.c_str());
  } catch (std::exception& e) {
    throwBadSyntax();
  }

  return ret;
}
