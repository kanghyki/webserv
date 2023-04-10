#include "./Parser.hpp"

Parser::Parser() {
  pos = 0;
};

Parser::~Parser() {};

void Parser::NextToken() {
  if (this->pos + 1 < tokens.size()) {
    this->pos += 1;
  }
};

Token Parser::CurToken() {
  return this->tokens[this->pos];
};

// FIXME:
Token Parser::PeekToken() {
  return this->tokens[this->pos + 1];
}

void Parser::expectTokenType(std::string expected) {
  NextToken();
  if (CurToken().type != expected)
    throw std::exception();
}

void Parser::generateToken(std::string fileName) {
  std::ifstream fileIn(fileName.c_str(), std::ifstream::in);
  std::string line;
  Lexer l;
  Token t;

  if (!fileIn.is_open())
    throw std::exception();

  while (!std::getline(fileIn, line).eof()) {
    l.SetInput(line);
    t = l.NextToken();
    while (t.type != token_type::END_OF_FILE)
      tokens.push_back(t);
  }
  tokens.push_back(Token(token_type::END_OF_FILE, ""));
}

Config Parser::Parse(const std::string fileName) {

  generateToken(fileName);
  while (CurToken().type != token_type::END_OF_FILE) {
    if (CurToken().type == token_type::HTTP) {
      HttpConfig http;
      ParseHttp(http);
      this->conf.addHttpConfig(http);
    } else {
      throw ParseException();
    }
    NextToken();
  }

  return this->conf;
}

void Parser::ParseHttp(HttpConfig conf) {
  expectTokenType(token_type::LBRACE);

  NextToken();
  while (CurToken().type != token_type::END_OF_FILE
      && CurToken().type != token_type::RBRACE) {
    if (CurToken().type == token_type::SERVER) {
      ServerConfig server;
      ParseServer(server);
      conf.addServerConfig(server);
    } else {
      if (CurToken().type == token_type::ROOT
          || CurToken().type == token_type::CLIENT_BODY_BUFFER_SIZE
          || CurToken().type == token_type::ERROR_PAGE
          || CurToken().type == token_type::INDEX) {
        ParseCommon(conf);
      }
      expectTokenType(token_type::SEMICOLON);
    }
    NextToken();
  }

  if (CurToken().type != token_type::RBRACE)
    throw RbraceException();
}

void Parser::ParseServer(ServerConfig conf) {
  expectTokenType(token_type::LBRACE);

  NextToken();
  while (CurToken().type != token_type::END_OF_FILE
      && CurToken().type != token_type::RBRACE) {
    if (CurToken().type == token_type::LOCATION) {
      LocationConfig location;
      ParseLocation(location);
      conf.addLocationConfig(location);
    }
    else {
      if (CurToken().type == token_type::ROOT
          || CurToken().type == token_type::CLIENT_BODY_BUFFER_SIZE
          || CurToken().type == token_type::ERROR_PAGE
          || CurToken().type == token_type::INDEX) {
        ParseCommon(conf);
      }
      else if (CurToken().type == token_type::LISTEN) {
        NextToken();
        std::vector<std::string> sp = Split(CurToken().literal, ":");
        if (sp.size() != 2)
          throw ParseException();
        // TODO: check arguments
        conf.setHost(sp[0]);
        conf.setPort(std::atoi(sp[1].c_str()));
      }
      else if (CurToken().type == token_type::SERVER_NAME) {
        NextToken();
        conf.setServerName(CurToken().literal);
      }
      expectTokenType(token_type::SEMICOLON);
    }
    NextToken();
  }

  if (CurToken().type != token_type::RBRACE)
    throw RbraceException();
}

void Parser::ParseLocation(LocationConfig conf) {
  NextToken();
  conf.setPath(CurToken().literal);

  expectTokenType(token_type::LBRACE);

  NextToken();
  while (CurToken().type != token_type::END_OF_FILE
      && CurToken().type != token_type::RBRACE) {
    if (CurToken().type == token_type::LOCATION) {
      LocationConfig location;
      ParseLocation(location);
      conf.addLocationConfig(location);
    } else {
      if (CurToken().type == token_type::ROOT
          || CurToken().type == token_type::CLIENT_BODY_BUFFER_SIZE
          || CurToken().type == token_type::ERROR_PAGE
          || CurToken().type == token_type::INDEX) {
        ParseCommon(conf);
      } else if (CurToken().type == token_type::ALIAS) {
        NextToken();
        conf.setAlias(CurToken().literal);
      } else if (CurToken().type == token_type::LIMIT_EXCEPT) {
        NextToken();
        conf.setLimitExcept(CurToken().literal);
      } else if (CurToken().type == token_type::AUTO_INDEX) {
        NextToken();
        if (CurToken().literal == "on")
          conf.setAutoIndex(true);
        else
          conf.setAutoIndex(false);
      } else if (CurToken().type == token_type::RETURN) {
        NextToken();
        int status_code = std::atoi(CurToken().literal.c_str());
        NextToken();
        conf.addReturnRes(std::pair<int, std::string>(status_code, CurToken().literal));
      }
      expectTokenType(token_type::SEMICOLON);
    }
    NextToken();
  }

  if (CurToken().type != token_type::RBRACE)
    throw RbraceException();
}

void Parser::ParseCommon(CommonConfig conf) {
  if (CurToken().type == token_type::ROOT) {
    expectTokenType(token_type::IDENT);
    conf.setRoot(CurToken().literal);
  }
  else if (CurToken().type == token_type::ERROR_PAGE) {
    NextToken();
    int http_status = std::atoi(CurToken().literal.c_str());
    expectTokenType(token_type::IDENT);
    conf.addErrorPage(std::pair<int, std::string>(http_status, CurToken().literal));
  }
  else if (CurToken().type == token_type::CLIENT_BODY_BUFFER_SIZE) {
    expectTokenType(token_type::INT);
    conf.setClientBodySize(std::atoi(CurToken().literal.c_str()));
  }
  else if (CurToken().type == token_type::INDEX) {
    while (PeekToken().type != token_type::SEMICOLON) {
      NextToken();
      conf.addIndex(CurToken().literal);
    }
  }
}

const char *Parser::RbraceException::what() const throw() {
  return "right brace error";
}

const char *Parser::ParseException::what() const throw() {
  return "parse error";
}

std::vector<std::string> Parser::Split(std::string line, std::string sep) {
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
