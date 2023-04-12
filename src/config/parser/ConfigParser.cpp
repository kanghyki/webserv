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

//FIXME:: 경계조건 에ㅓ러 
Token ConfigParser::prevToken() const {
  return this->tokens[this->pos - 1];
}

// FIXME:
Token ConfigParser::peekToken() const {
  return this->tokens[this->pos + 1];
}

// FIXME : to_string(CPP11) 사용중
// TODO: refactor
void ConfigParser::expectNextToken(std::string expected) {
  nextToken();

  if (curToken().getType() != expected) {
    std::string errorMsg =
      this->fileName
      + " "
      + std::to_string(prevToken().getLineNumber())
      + ":" + std::to_string(prevToken().getPos() + prevToken().getLiteral().length() + 1)
      + " expected \'" + expected + "\' but ";
    if (prevToken().getLineNumber() == curToken().getLineNumber()) {
      errorMsg += "\'" + curToken().getLiteral() + "\'\n";
    }
    else {
      errorMsg += "nothing\n";
    }

    std::string endMsg = "\nconfig error occured.";

    throw std::invalid_argument(errorMsg + endMsg);
  }
}

// FIXME : to_string(CPP11) 사용중
// TODO: refactor
void ConfigParser::expectCurToken(std::string expected) {
  if (curToken().getType() != expected) {
    std::string errorMsg =
      this->fileName
      + " "
      + std::to_string(prevToken().getLineNumber())
      + ":" + std::to_string(prevToken().getPos() + prevToken().getLiteral().length() + 1)
      + " expected \'" + expected + "\' but ";
    if (prevToken().getLineNumber() == curToken().getLineNumber()) {
      errorMsg += "\'" + curToken().getLiteral() + "\'\n";
    }
    else {
      errorMsg += "nothing\n";
    }
    std::string endMsg = "\nconfig error occured.";

    throw std::invalid_argument(errorMsg + endMsg);
  }
}

// TODO: refactor
void ConfigParser::badSyntax() const {
  std::string errorMsg =
    this->fileName
    + " "
    + std::to_string(curToken().getLineNumber())
    + ":" + std::to_string(curToken().getPos() + prevToken().getLiteral().length() + 1)
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
    while (t.isNot(token_type::END_OF_FILE)) {
      tokens.push_back(t);
      t = l.nextToken();
      t.setLineNumber(lineCount);
    }
    lineCount += 1;
  }
  tokens.push_back(Token(token_type::END_OF_FILE, ""));
}

Config ConfigParser::Parse(const std::string &fileName) {
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

void ConfigParser::parseHttp(HttpConfig &conf) {
  expectNextToken(token_type::LBRACE);

  nextToken();
  while (curToken().isNot(token_type::END_OF_FILE)
      && curToken().isNot(token_type::RBRACE)) {
    // [ server ]
    if (curToken().is(token_type::SERVER)) {
      ServerConfig server;
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

void ConfigParser::parseServer(ServerConfig &conf) {
  expectNextToken(token_type::LBRACE);

  nextToken();
  while (curToken().isNot(token_type::END_OF_FILE)
      && curToken().isNot(token_type::RBRACE)) {
    // [ location ]
    if (curToken().is(token_type::LOCATION)) {
      LocationConfig location;
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
        std::vector<std::string> sp = splitStr(curToken().getLiteral(), ":");
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

void ConfigParser::parseLocation(LocationConfig &conf) {
  expectNextToken(token_type::IDENT);
  conf.setPath(curToken().getLiteral());

  expectNextToken(token_type::LBRACE);

  nextToken();
  while (curToken().isNot(token_type::END_OF_FILE)
      && curToken().isNot(token_type::RBRACE)) {
    // [ location ]
    if (curToken().is(token_type::LOCATION)) {
      LocationConfig location;
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
    else if (curToken().is(token_type::AUTO_INDEX)) {
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

void ConfigParser::parseCommon(CommonConfig &conf) {
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
