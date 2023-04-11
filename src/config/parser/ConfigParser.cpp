#include "./ConfigParser.hpp"

ConfigParser::ConfigParser() {
  pos = 0;
};

ConfigParser::~ConfigParser() {};

void ConfigParser::nextToken() {
  if (this->pos + 1 < tokens.size()) {
    this->pos += 1;
  }
};

Token ConfigParser::curToken() {
  return this->tokens[this->pos];
};

//FIXME:: 경계조건 에ㅓ러 
Token ConfigParser::prevToken() {
  return this->tokens[this->pos - 1];
}

// FIXME:
Token ConfigParser::peekToken() {
  return this->tokens[this->pos + 1];
}

void ConfigParser::expectTokenType(std::string expected) {
  nextToken();

  // FIXME : CPP11 사용중
  // TODO : 에러 라인 출력
  if (curToken().getType() != expected) {
    std::string errorMsg = "server.conf|"\
    + std::to_string(prevToken().getLine()) + ":" + std::to_string(prevToken().getWord())
    + "|expected \'" + expected + "\'\n";
    std::string endMsg = "\nconfig error occured.";
    throw std::invalid_argument(errorMsg + endMsg);
  }
}

void ConfigParser::generateToken(std::string fileName) {
  size_t lineCount = 1;
  std::ifstream fileIn(fileName.c_str(), std::ifstream::in);
  std::string line;
  Lexer l;
  Token t;

  if (!fileIn.is_open())
    throw std::exception();

  while (!std::getline(fileIn, line).eof()) {
    l.setInput(line);
    t = l.nextToken();
    t.setLine(lineCount);
    t.setWord(l.getPosition() + 1);
    while (t.isNotType(token_type::END_OF_FILE)) {
      tokens.push_back(t);
      t = l.nextToken();
      t.setLine(lineCount);
      t.setWord(l.getPosition() + 1);
    }
    lineCount += 1;
  }
  tokens.push_back(Token(token_type::END_OF_FILE, ""));
}

Config ConfigParser::Parse(const std::string &fileName) {
  Config conf;

  generateToken(fileName);
  while (curToken().isNotType(token_type::END_OF_FILE)) {
    if (curToken().isType(token_type::HTTP)) {
      HttpConfig http;
      parseHttp(http);
      conf.addHttpConfig(http);
    }
    else
      throw ParseException();
    nextToken();
  }

  return conf;
}

void ConfigParser::parseHttp(HttpConfig &conf) {
  expectTokenType(token_type::LBRACE);

  nextToken();
  while (curToken().isNotType(token_type::END_OF_FILE)
      && curToken().isNotType(token_type::RBRACE)) {
    if (curToken().isType(token_type::SERVER)) {
      ServerConfig server;
      parseServer(server);
      conf.addServerConfig(server);
    }
    else {
      if (curToken().isCommon())
        parseCommon(conf);
      expectTokenType(token_type::SEMICOLON);
    }
    nextToken();
  }

  if (curToken().getType() != token_type::RBRACE)
    throw RbraceException();
}

void ConfigParser::parseServer(ServerConfig &conf) {
  expectTokenType(token_type::LBRACE);

  nextToken();
  while (curToken().isNotType(token_type::END_OF_FILE)
      && curToken().isNotType(token_type::RBRACE)) {
    if (curToken().isType(token_type::LOCATION)) {
      LocationConfig location;
      parseLocation(location);
      conf.addLocationConfig(location);
    }
    else {
      if (curToken().isCommon()) {
        parseCommon(conf);
      }
      else if (curToken().isListen()) {
        nextToken();
        std::vector<std::string> sp = splitStr(curToken().getLiteral(), ":");
        // FIXME: 포트만 들어오는 경우도 있음
        if (sp.size() != 2) {
          throw ParseException();
        }
        // TODO: check arguments
        conf.setHost(sp[0]);
        conf.setPort(std::atoi(sp[1].c_str()));
      }
      else if (curToken().isServerName()) {
        nextToken();
        conf.setServerName(curToken().getLiteral());
      }
      expectTokenType(token_type::SEMICOLON);
    }
    nextToken();
  }

  if (curToken().isNotType(token_type::RBRACE))
    throw RbraceException();
}

void ConfigParser::parseLocation(LocationConfig &conf) {
  nextToken();
  conf.setPath(curToken().getLiteral());

  expectTokenType(token_type::LBRACE);

  nextToken();
  while (curToken().isNotType(token_type::END_OF_FILE)
      && curToken().isNotType(token_type::RBRACE)) {
    if (curToken().isType(token_type::LOCATION)) {
      LocationConfig location;
      parseLocation(location);
      conf.addLocationConfig(location);
    }
    else {
      if (curToken().isCommon()) {
        parseCommon(conf);
      }
      else if (curToken().isAlias()) {
        nextToken();
        conf.setAlias(curToken().getLiteral());
      }
      else if (curToken().isLimitExcept()) {
        nextToken();
        conf.setLimitExcept(curToken().getLiteral());
      }
      else if (curToken().isAutoIndex()) {
        nextToken();
        if (curToken().getLiteral() == "on")
          conf.setAutoIndex(true);
        else if (curToken().getLiteral() == "off")
          conf.setAutoIndex(false);
      }
      else if (curToken().isRoot()) {
        nextToken();
        int status_code = std::atoi(curToken().getLiteral().c_str());
        nextToken();
        conf.addReturnRes(std::pair<int, std::string>(status_code, curToken().getLiteral()));
      }
      expectTokenType(token_type::SEMICOLON);
    }
    nextToken();
  }

  if (curToken().isNotType(token_type::RBRACE))
    throw RbraceException();
}

void ConfigParser::parseCommon(CommonConfig &conf) {
  if (curToken().isRoot()) {
    expectTokenType(token_type::IDENT);
    conf.setRoot(curToken().getLiteral());
  }
  else if (curToken().isErrorPage()) {
    expectTokenType(token_type::IDENT);
    int http_status = std::atoi(curToken().getLiteral().c_str());
    expectTokenType(token_type::IDENT);
    conf.addErrorPage(std::pair<int, std::string>(http_status, curToken().getLiteral()));
  }
  else if (curToken().isClientBodyBufferSize()) {
    expectTokenType(token_type::IDENT);
    conf.setClientBodySize(std::atoi(curToken().getLiteral().c_str()));
  }
  else if (curToken().isIndex()) {
    while (peekToken().isType(token_type::IDENT)) {
      nextToken();
      conf.addIndex(curToken().getLiteral());
    }
  }
}

const char *ConfigParser::RbraceException::what() const throw() {
  return "right brace error";
}

const char *ConfigParser::ParseException::what() const throw() {
  return "parse error";
}

std::vector<std::string> ConfigParser::splitStr(std::string line, std::string sep) {
  std::vector<std::string> ret;
  unsigned long pos = 0;
  std::string token = "";

  while ((pos = line.find(sep)) != std::string::npos)
  {
    token = line.substr(0, pos);
    ret.push_back(token);
    line.erase(0, pos + sep.length());
  }
  ret.push_back(line);
  return ret;
}
