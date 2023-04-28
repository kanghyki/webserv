#include "./ConfigParser.hpp"

ConfigParser::ConfigParser() {
  pos = 0;
};

ConfigParser::~ConfigParser() {};

Config ConfigParser::parse(const std::string& fileName) throw(std::runtime_error) {
  Config conf;

  generateToken(fileName);
  while (curToken().isNot(Token::END_OF_FILE)) {
    if (curToken().is(Token::HTTP)) conf.addHttpConfig(parseHttp());
    else throwBadSyntax();
    nextToken();
  }
  expectCurToken(Token::END_OF_FILE);

  return conf;
}

HttpConfig ConfigParser::parseHttp() {
  HttpConfig conf;

  expectNextToken(Token::LBRACE);
  for (nextToken(); curToken().isNot(Token::END_OF_FILE) && curToken().isNot(Token::RBRACE); nextToken()) {
    if (curToken().is(Token::SERVER)) conf.addServerConfig(parseServer(conf));
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
    else if (curToken().is(Token::LISTEN)) parseListen(conf);
    else if (curToken().is(Token::SERVER_NAME)) parseServerName(conf);
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
    else if (curToken().is(Token::ALIAS)) parseAlias(conf);
    else if (curToken().is(Token::LIMIT_EXCEPT)) parseLimitExcept(conf);
    else if (curToken().is(Token::CGI)) parseCGI(conf);
    else if (curToken().is(Token::AUTOINDEX)) parseAutoIndex(conf);
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
    else if (curToken().is(Token::ALIAS)) parseAlias(conf);
    else if (curToken().is(Token::LIMIT_EXCEPT)) parseLimitExcept(conf);
    else if (curToken().is(Token::CGI)) parseCGI(conf);
    else if (curToken().is(Token::AUTOINDEX)) parseAutoIndex(conf);
    else if (curToken().is(Token::RETURN)) parseReturn(conf);
    else throwBadSyntax();
  }
  expectCurToken(Token::RBRACE);

  return conf;
}

void ConfigParser::parseCommon(CommonConfig& conf) {
  if (curToken().is(Token::ROOT)) parseRoot(conf);
  else if (curToken().is(Token::ERROR_PAGE)) parseErrorPage(conf);
  else if (curToken().is(Token::CLIENT_BODY_BUFFER_SIZE)) parseClientBodyBufferSize(conf);
  else if (curToken().is(Token::TIMEOUT)) parseTimeout(conf);
  else if (curToken().is(Token::INDEX)) parseIndex(conf);
}

// server

void ConfigParser::parseListen(ServerConfig& conf) {
  nextToken();
  // HOST:PORT
  if (curToken().is(Token::IDENT)) {
    std::vector<std::string> sp = util::split(curToken().getLiteral(), ":");
    // FIXME: 포트만 들어오는 경우도 있음
    if (sp.size() != 2) throw std::runtime_error("Listen error");
    // TODO: check arguments
    conf.setHost(sp[0]);
    conf.setPort(std::atoi(sp[1].c_str()));
  }
  // PORT
  else if (curToken().is(Token::INT)) {
    conf.setPort(std::atoi(curToken().getLiteral().c_str()));
  }
  expectNextToken(Token::SEMICOLON);
}

void ConfigParser::parseServerName(ServerConfig& conf) {
  expectNextToken(Token::IDENT);
  conf.setServerName(curToken().getLiteral());
  expectNextToken(Token::SEMICOLON);
}

// location

void ConfigParser::parseAlias(LocationConfig& conf) {
  expectNextToken(Token::IDENT);
  conf.setAlias(curToken().getLiteral());
  expectNextToken(Token::SEMICOLON);
}

void ConfigParser::parseLimitExcept(LocationConfig& conf) {
  expectNextToken(Token::IDENT);
  conf.setLimitExcept(curToken().getLiteral());
  expectNextToken(Token::SEMICOLON);
}

void ConfigParser::parseCGI(LocationConfig& conf) {
  expectNextToken(Token::IDENT);
  conf.setCGIScriptPath(curToken().getLiteral());
  expectNextToken(Token::IDENT);
  conf.setCGIPath(curToken().getLiteral());
  expectNextToken(Token::SEMICOLON);
}

void ConfigParser::parseAutoIndex(LocationConfig& conf) {
  expectNextToken(Token::IDENT);
  if (curToken().getLiteral() == "on")
    conf.setAutoIndex(true);
  else if (curToken().getLiteral() == "off")
    conf.setAutoIndex(false);
  else
    throw std::runtime_error("AutoIndex error");
  expectNextToken(Token::SEMICOLON);
}

void ConfigParser::parseReturn(LocationConfig& conf) {
  expectNextToken(Token::INT);
  int status_code = std::atoi(curToken().getLiteral().c_str());
  expectNextToken(Token::IDENT);
  conf.addReturnRes(std::pair<int, std::string>(status_code, curToken().getLiteral()));
  expectNextToken(Token::SEMICOLON);
}

// common

void ConfigParser::parseRoot(CommonConfig& conf) {
  expectNextToken(Token::IDENT);
  conf.setRoot(curToken().getLiteral());
  expectNextToken(Token::SEMICOLON);
}

void ConfigParser::parseErrorPage(CommonConfig& conf) {
  std::vector<int> statusList;

  while (peekToken().is(Token::INT)) {
    nextToken();
    int status = std::atoi(curToken().getLiteral().c_str());
    statusList.push_back(status);
  }
  expectNextToken(Token::IDENT);

  for (int i = 0; i < statusList.size(); ++i)
    conf.addErrorPage(statusList[i], curToken().getLiteral());
  expectNextToken(Token::SEMICOLON);
}

void ConfigParser::parseClientBodyBufferSize(CommonConfig& conf) {
  expectNextToken(Token::INT);
  conf.setClientBodySize(std::atoi(curToken().getLiteral().c_str()));
  expectNextToken(Token::SEMICOLON);
}

void ConfigParser::parseTimeout(CommonConfig& conf) {
  expectNextToken(Token::INT);
  conf.setTimeout(std::atoi(curToken().getLiteral().c_str()));
  expectNextToken(Token::SEMICOLON);
}

void ConfigParser::parseIndex(CommonConfig& conf) {
  while (peekToken().is(Token::IDENT)) {
    expectNextToken(Token::IDENT);
    conf.addIndex(curToken().getLiteral());
  }
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
  if (this->pos > this->tokens.size())
    throw std::runtime_error("tokenization error");
  return this->tokens[this->pos + 1];
}

void ConfigParser::expectNextToken(const std::string& expected) {
  nextToken();

  if (curToken().getType() != expected)
    expectError(expected);
}

void ConfigParser::expectCurToken(const std::string& expected) const {
  if (curToken().getType() != expected)
    expectError(expected);
}

// TODO:: move
std::string itoa(int n) {
  std::stringstream ss;

  ss << n;

  return ss.str();
}

void ConfigParser::expectError(const std::string& expected) const {
  std::string errorMsg =
    this->fileName
    + " "
    + std::to_string(curToken().getLineNumber())
    + ":" + itoa(curToken().getPos())
    + " expected \'" + expected + "\' but \'" + curToken().getLiteral() + "\'";

  throw std::runtime_error(errorMsg);
}

void ConfigParser::throwBadSyntax() const {
  std::string errorMsg =
    this->fileName
    + " "
    + std::to_string(curToken().getLineNumber())
    + ":" + std::to_string(curToken().getPos())
    + " bad syntax \'" + curToken().getLiteral() + "\'";

  throw std::runtime_error(errorMsg);
}
