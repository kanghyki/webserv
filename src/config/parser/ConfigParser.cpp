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

Token ConfigParser::curToken() const {
  return this->tokens[this->pos];
};

Token ConfigParser::peekToken() const {
  if (this->pos > this->tokens.size())
    return this->tokens[this->tokens.size() - 1];
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

// FIXME : to_string(CPP11) 사용중
void ConfigParser::expectError(const std::string& expected) const {
  std::string errorMsg =
    this->fileName
    + " "
    + std::to_string(curToken().getLineNumber())
    + ":" + std::to_string(curToken().getPos())
    + " expected \'" + expected + "\' but \'" + curToken().getLiteral() + "\'\n"
    + "\nconfig error occured.";

  throw std::invalid_argument(errorMsg);
}

// TODO: refactor
void ConfigParser::badSyntax() const {
  std::string errorMsg =
    this->fileName
    + " "
    + std::to_string(curToken().getLineNumber())
    + ":" + std::to_string(curToken().getPos())
    + " bad syntax \'" + curToken().getLiteral() + "\'";

  throw std::invalid_argument(errorMsg);
}

void ConfigParser::generateToken(std::string fileName) {
  size_t lineCount = 1;
  std::ifstream fileIn(fileName.c_str(), std::ifstream::in);
  std::string line;
  Lexer l;
  Token t;

  this->fileName = fileName;
  if (!fileIn.is_open())
    throw std::exception();

  while (!std::getline(fileIn, line).eof()) {
    l.setInput(line);
    t = l.nextToken();
    t.setLineNumber(lineCount);
    // TODO:: END_OF_LINE TOKEN 추가
    // TODO:: 로직 수정
    while (t.isNot(token_type::END_OF_FILE)) {
      tokens.push_back(t);
      t = l.nextToken();
      t.setLineNumber(lineCount);
    }
    lineCount += 1;
  }
  t = Token(token_type::END_OF_FILE, "END_OF_FILE");
  t.setLineNumber(lineCount);
  tokens.push_back(t);
}

Config ConfigParser::Parse(const std::string& fileName) {
  Config conf;

  generateToken(fileName);
  while (curToken().isNot(token_type::END_OF_FILE)) {
    // [ http ]
    if (curToken().is(token_type::HTTP)) {
      HttpConfig http;
      parseHttp(http);
      conf.addHttpConfig(http);
    }
    else {
      badSyntax();
    }
    nextToken();
  }

  expectCurToken(token_type::END_OF_FILE);

  return conf;
}

void ConfigParser::parseHttp(HttpConfig& conf) {
  expectNextToken(token_type::LBRACE);

  nextToken();
  while (curToken().isNot(token_type::END_OF_FILE)
      && curToken().isNot(token_type::RBRACE)) {
    // [ server ]
    if (curToken().is(token_type::SERVER)) {
      ServerConfig server(conf);
      parseServer(server);
      conf.addServerConfig(server);
    }
      // [ common ]
    else if (curToken().isCommon()) {
      parseCommon(conf);
      expectNextToken(token_type::SEMICOLON);
    }
    else {
      badSyntax();
    }
    nextToken();
  }

  expectCurToken(token_type::RBRACE);
}

void ConfigParser::parseServer(ServerConfig& conf) {
  expectNextToken(token_type::LBRACE);

  nextToken();
  while (curToken().isNot(token_type::END_OF_FILE)
      && curToken().isNot(token_type::RBRACE)) {
    // [ location ]
    if (curToken().is(token_type::LOCATION)) {
      LocationConfig location(conf);
      parseLocation(location);
      conf.addLocationConfig(location);
    }
    // [ common ]
    else if (curToken().isCommon()) {
      parseCommon(conf);
      expectNextToken(token_type::SEMICOLON);
    }
    // listen
    else if (curToken().is(token_type::LISTEN)) {
      nextToken();
      // HOST:PORT
      if (curToken().is(token_type::IDENT)) {
        std::vector<std::string> sp = util::split(curToken().getLiteral(), ":");
        // FIXME: 포트만 들어오는 경우도 있음
        if (sp.size() != 2) {
          throw ParseException();
        }
        // TODO: check arguments
        conf.setHost(sp[0]);
        conf.setPort(std::atoi(sp[1].c_str()));
      }
      // PORT
      else if (curToken().is(token_type::INT)) {
        conf.setPort(std::atoi(curToken().getLiteral().c_str()));
      }
      expectNextToken(token_type::SEMICOLON);
    }
    // server_name
    else if (curToken().is(token_type::SERVER_NAME)) {
      expectNextToken(token_type::IDENT);
      conf.setServerName(curToken().getLiteral());
      expectNextToken(token_type::SEMICOLON);
    }
    else {
      badSyntax();
    }
    nextToken();
  }
  expectCurToken(token_type::RBRACE);
}

void ConfigParser::parseLocation(LocationConfig& conf) {
  expectNextToken(token_type::IDENT);
  conf.setPath(curToken().getLiteral());

  expectNextToken(token_type::LBRACE);

  nextToken();
  while (curToken().isNot(token_type::END_OF_FILE)
      && curToken().isNot(token_type::RBRACE)) {
    // [ location ]
    if (curToken().is(token_type::LOCATION)) {
      LocationConfig location(conf);
      parseLocation(location);
      conf.addLocationConfig(location);
    }
    // [ common ]
    else if (curToken().isCommon()) {
      parseCommon(conf);
      expectNextToken(token_type::SEMICOLON);
    }
    // alias
    else if (curToken().is(token_type::ALIAS)) {
      expectNextToken(token_type::IDENT);
      conf.setAlias(curToken().getLiteral());
      expectNextToken(token_type::SEMICOLON);
    }
    // limit_except
    else if (curToken().is(token_type::LIMIT_EXCEPT)) {
      expectNextToken(token_type::IDENT);
      conf.setLimitExcept(curToken().getLiteral());
      expectNextToken(token_type::SEMICOLON);
    }
    // auto_index
    else if (curToken().is(token_type::AUTOINDEX)) {
      expectNextToken(token_type::IDENT);
      // FIXME: on off else
      if (curToken().getLiteral() == "on")
        conf.setAutoIndex(true);
      else if (curToken().getLiteral() == "off")
        conf.setAutoIndex(false);
      expectNextToken(token_type::SEMICOLON);
    }
    // return
    else if (curToken().is(token_type::RETURN)) {
      expectNextToken(token_type::INT);
      int status_code = std::atoi(curToken().getLiteral().c_str());
      expectNextToken(token_type::IDENT);
      conf.addReturnRes(std::pair<int, std::string>(status_code, curToken().getLiteral()));
      expectNextToken(token_type::SEMICOLON);
    }
    else {
      badSyntax();
    }
    nextToken();
  }
  expectCurToken(token_type::RBRACE);
}

void ConfigParser::parseCommon(CommonConfig& conf) {
  // root
  if (curToken().is(token_type::ROOT)) {
    expectNextToken(token_type::IDENT);
    conf.setRoot(curToken().getLiteral());
  }
  // error page
  else if (curToken().is(token_type::ERROR_PAGE)) {
    expectNextToken(token_type::INT);
    int http_status = std::atoi(curToken().getLiteral().c_str());
    expectNextToken(token_type::IDENT);
    conf.addErrorPage(std::pair<int, std::string>(http_status, curToken().getLiteral()));
  }
  // client_body_buffer_size
  else if (curToken().is(token_type::CLIENT_BODY_BUFFER_SIZE)) {
    expectNextToken(token_type::INT);
    conf.setClientBodySize(std::atoi(curToken().getLiteral().c_str()));
  }
  // index
  else if (curToken().is(token_type::INDEX)) {
    while (peekToken().is(token_type::IDENT)) {
      expectNextToken(token_type::IDENT);
      conf.addIndex(curToken().getLiteral());
    }
  }
}

const char *ConfigParser::ParseException::what() const throw() {
  return "parse error";
}
