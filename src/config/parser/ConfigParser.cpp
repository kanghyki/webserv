#include "./ConfigParser.hpp"

ConfigParser::ConfigParser() {
  pos = 0;
};

ConfigParser::~ConfigParser() {};

void ConfigParser::NextToken() {
  if (this->pos + 1 < tokens.size()) {
    this->pos += 1;
  }
};

Token ConfigParser::CurToken() {
  return this->tokens[this->pos];
};

// FIXME:
Token ConfigParser::PeekToken() {
  return this->tokens[this->pos + 1];
}

void ConfigParser::expectTokenType(std::string expected) {
  NextToken();
  if (CurToken().getType() != expected)
    throw std::invalid_argument("=> " + expected);
}

void ConfigParser::generateToken(std::string fileName) {
  std::ifstream fileIn(fileName.c_str(), std::ifstream::in);
  std::string line;
  Lexer l;
  Token t;

  if (!fileIn.is_open())
    throw std::exception();

  while (!std::getline(fileIn, line).eof()) {
    l.SetInput(line);
    while (!(t = l.NextToken()).isEqaulType(token_type::END_OF_FILE)) {
      tokens.push_back(t);
    }
  }
  tokens.push_back(Token(token_type::END_OF_FILE, ""));
}

Config ConfigParser::Parse(const std::string &fileName) {
  Config conf;

  generateToken(fileName);
  while (CurToken().getType() != token_type::END_OF_FILE) {
    if (CurToken().getType() == token_type::HTTP) {
      HttpConfig http;
      ParseHttp(http);
      conf.addHttpConfig(http);
    } else {
      throw ParseException();
    }
    NextToken();
  }

  return conf;
}

void ConfigParser::ParseHttp(HttpConfig &conf) {
  expectTokenType(token_type::LBRACE);

  NextToken();
  while (CurToken().getType() != token_type::END_OF_FILE
      && CurToken().getType() != token_type::RBRACE) {
    if (CurToken().getType() == token_type::SERVER) {
      ServerConfig server;
      ParseServer(server);
      conf.addServerConfig(server);
    } else {
      if (CurToken().getType() == token_type::ROOT
          || CurToken().getType() == token_type::CLIENT_BODY_BUFFER_SIZE
          || CurToken().getType() == token_type::ERROR_PAGE
          || CurToken().getType() == token_type::INDEX) {
        ParseCommon(conf);
      }
      expectTokenType(token_type::SEMICOLON);
    }
    NextToken();
  }

  if (CurToken().getType() != token_type::RBRACE)
    throw RbraceException();
}

void ConfigParser::ParseServer(ServerConfig &conf) {
  expectTokenType(token_type::LBRACE);

  NextToken();
  while (CurToken().getType() != token_type::END_OF_FILE
      && CurToken().getType() != token_type::RBRACE) {
    if (CurToken().getType() == token_type::LOCATION) {
      LocationConfig location;
      ParseLocation(location);
      conf.addLocationConfig(location);
    }
    else {
      if (CurToken().getType() == token_type::ROOT
          || CurToken().getType() == token_type::CLIENT_BODY_BUFFER_SIZE
          || CurToken().getType() == token_type::ERROR_PAGE
          || CurToken().getType() == token_type::INDEX) {
        ParseCommon(conf);
      }
      else if (CurToken().getType() == token_type::LISTEN) {
        NextToken();
        std::vector<std::string> sp = Split(CurToken().getLiteral(), ":");
        // FIXME: 포트만 들어오는 경우도 있음
        if (sp.size() != 2) {
          throw ParseException();
        }
        // TODO: check arguments
        conf.setHost(sp[0]);
        conf.setPort(std::atoi(sp[1].c_str()));
      }
      else if (CurToken().getType() == token_type::SERVER_NAME) {
        NextToken();
        conf.setServerName(CurToken().getLiteral());
      }
      expectTokenType(token_type::SEMICOLON);
    }
    NextToken();
  }

  if (CurToken().getType() != token_type::RBRACE)
    throw RbraceException();
}

void ConfigParser::ParseLocation(LocationConfig &conf) {
  NextToken();
  conf.setPath(CurToken().getLiteral());

  expectTokenType(token_type::LBRACE);

  NextToken();
  while (CurToken().getType() != token_type::END_OF_FILE
      && CurToken().getType() != token_type::RBRACE) {
    if (CurToken().getType() == token_type::LOCATION) {
      LocationConfig location;
      ParseLocation(location);
      conf.addLocationConfig(location);
    }
    else {
      if (CurToken().getType() == token_type::ROOT
          || CurToken().getType() == token_type::CLIENT_BODY_BUFFER_SIZE
          || CurToken().getType() == token_type::ERROR_PAGE
          || CurToken().getType() == token_type::INDEX) {
        ParseCommon(conf);
      }
      else if (CurToken().getType() == token_type::ALIAS) {
        NextToken();
        conf.setAlias(CurToken().getLiteral());
      }
      else if (CurToken().getType() == token_type::LIMIT_EXCEPT) {
        NextToken();
        conf.setLimitExcept(CurToken().getLiteral());
      }
      else if (CurToken().getType() == token_type::AUTO_INDEX) {
        NextToken();
        if (CurToken().getLiteral() == "on")
          conf.setAutoIndex(true);
        else
          conf.setAutoIndex(false);
      }
      else if (CurToken().getType() == token_type::RETURN) {
        NextToken();
        int status_code = std::atoi(CurToken().getLiteral().c_str());
        NextToken();
        conf.addReturnRes(std::pair<int, std::string>(status_code, CurToken().getLiteral()));
      }
      expectTokenType(token_type::SEMICOLON);
    }
    NextToken();
  }

  if (CurToken().getType() != token_type::RBRACE)
    throw RbraceException();
}

void ConfigParser::ParseCommon(CommonConfig &conf) {
  if (CurToken().getType() == token_type::ROOT) {
    expectTokenType(token_type::IDENT);
    conf.setRoot(CurToken().getLiteral());
  }
  else if (CurToken().getType() == token_type::ERROR_PAGE) {
    expectTokenType(token_type::IDENT);
    int http_status = std::atoi(CurToken().getLiteral().c_str());
    expectTokenType(token_type::IDENT);
    conf.addErrorPage(std::pair<int, std::string>(http_status, CurToken().getLiteral()));
  }
  else if (CurToken().getType() == token_type::CLIENT_BODY_BUFFER_SIZE) {
    expectTokenType(token_type::IDENT);
    conf.setClientBodySize(std::atoi(CurToken().getLiteral().c_str()));
  }
  else if (CurToken().getType() == token_type::INDEX) {
    while (PeekToken().getType() == token_type::IDENT) {
      NextToken();
      conf.addIndex(CurToken().getLiteral());
    }
  }
}

const char *ConfigParser::RbraceException::what() const throw() {
  return "right brace error";
}

const char *ConfigParser::ParseException::what() const throw() {
  return "parse error";
}

std::vector<std::string> ConfigParser::Split(std::string line, std::string sep) {
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
