#include "parser.hpp"

Parser::Parser(): conf(new Configuration) {
  pos = 0;
};

Parser::~Parser() {};

void Parser::NextToken() {
  if (this->pos + 1 < t_arr.size()) {
    this->pos += 1;
  }
};

Token *Parser::CurToken() {
  return this->t_arr[this->pos];
};
// FIXME:
Token *Parser::PeekToken() {
  return this->t_arr[this->pos + 1];
}

void Parser::ReadAllToken(std::string file_name) {
  FileReader reader(file_name);
  Lexer l;
  Token *t;

  while (reader.IsReadable()) {
    std::string line;
    line = reader.ReadLine();
    //std::cout << line << std::endl;
    l.SetInput(line);

    while ((t = l.NextToken())->type != token_type::kEof) {
      //PrintToken(t);
      t_arr.push_back(t);
    }
    delete(t);
  }
  t_arr.push_back(new Token(token_type::kEof, ""));
}

Configuration *Parser::Parse(std::string file_name) {

  ReadAllToken(file_name);
  while (CurToken()->type != token_type:: kEof) {
    if (CurToken()->type == token_type::kHttp) {
      conf->http = new HttpConfiguration;
      ParseHttp(conf->http);
    } else {
      throw ParseException();
    }
    NextToken();
  }

  return this->conf;
}

void Parser::ParseHttp(HttpConfiguration *conf) {
  NextToken();
  if (CurToken()->type != token_type::kLbrace) {
    throw LbraceException();
  }

  NextToken();
  while (CurToken()->type != token_type::kEof
      && CurToken()->type != token_type::kRbrace) {
    if (CurToken()->type == token_type::kServer) {
      conf->servers.push_back(new ServerConfiguration);
      ParseServer(conf->servers.back());
    } else {
      if (CurToken()->type == token_type::kRoot
          || CurToken()->type == token_type::kClientBodyBufferSize
          || CurToken()->type == token_type::kErrorPage
          || CurToken()->type == token_type::kIndex) {
        ParseCommon(conf);
      }
      NextToken();
      if (CurToken()->type != token_type::kSemicolon) {
        throw SemicolonException();
      }
    }
    NextToken();
  }

  if (CurToken()->type != token_type::kRbrace) {
    throw RbraceException();
  }
}

void Parser::ParseServer(ServerConfiguration *conf) {
  NextToken();
  if (CurToken()->type != token_type::kLbrace) {
    throw LbraceException();
  }

  NextToken();
  while (CurToken()->type != token_type::kEof
      && CurToken()->type != token_type::kRbrace) {
    if (CurToken()->type == token_type::kLocation) {
      conf->locations.push_back(new LocationConfiguration);
      ParseLocation(conf->locations.back());
    } else {
      if (CurToken()->type == token_type::kRoot
          || CurToken()->type == token_type::kClientBodyBufferSize
          || CurToken()->type == token_type::kErrorPage
          || CurToken()->type == token_type::kIndex) {
        ParseCommon(conf);
      } else if (CurToken()->type == token_type::kListen) {
        NextToken();
        std::vector<std::string> sp = Split(CurToken()->literal, ":");
        if (sp.size() != 2) {
          throw ParseException();
        }
        // TODO: check arguments
        conf->host = sp[0];
        conf->port = std::stoi(sp[1]);
      } else if (CurToken()->type == token_type::kServerName) {
        NextToken();
        conf->server_name = CurToken()->literal;
      }
      NextToken();
      if (CurToken()->type != token_type::kSemicolon) {
        throw SemicolonException();
      }
    }
    NextToken();
  }

  if (CurToken()->type != token_type::kRbrace) {
    throw RbraceException();
  }
}

void Parser::ParseLocation(LocationConfiguration *conf) {
  NextToken();
  conf->path = CurToken()->literal;

  NextToken();
  if (CurToken()->type != token_type::kLbrace) {
    throw LbraceException();
  }

  NextToken();
  while (CurToken()->type != token_type::kEof
      && CurToken()->type != token_type::kRbrace) {
    if (CurToken()->type == token_type::kLocation) {
      conf->locations.push_back(new LocationConfiguration);
      ParseLocation(conf->locations.back());
    } else {
      if (CurToken()->type == token_type::kRoot
          || CurToken()->type == token_type::kClientBodyBufferSize
          || CurToken()->type == token_type::kErrorPage
          || CurToken()->type == token_type::kIndex) {
        ParseCommon(conf);
      } else if (CurToken()->type == token_type::kAlias) {
        NextToken();
        conf->alias = CurToken()->literal;
      } else if (CurToken()->type == token_type::kLimitExcept) {
        NextToken();
        conf->limit_except = CurToken()->literal;
      } else if (CurToken()->type == token_type::kAutoIndex) {
        NextToken();
        if (CurToken()->literal == "on") {
          conf->auto_index = true;
        } else {
          conf->auto_index = false;
        }
      } else if (CurToken()->type == token_type::kReturn) {
        NextToken();
        int status_code = stoi(CurToken()->literal);
        NextToken();
        conf->return_res.insert(std::pair<int, std::string>(status_code, CurToken()->literal));
      }
      NextToken();
      if (CurToken()->type != token_type::kSemicolon) {
        throw SemicolonException();
      }
    }
    NextToken();
  }

  if (CurToken()->type != token_type::kRbrace) {
    throw RbraceException();
  }
}

void Parser::ParseCommon(AConfiguration *conf) {
  if (CurToken()->type == token_type::kRoot) {
    NextToken();
    if (CurToken()->type != token_type::kIdent) {
      throw RootException();
    }
    conf->root = CurToken()->literal;
  } else if (CurToken()->type == token_type::kErrorPage) {
    NextToken();
    int http_status = std::stoi(CurToken()->literal);
    NextToken();
    if (CurToken()->type != token_type::kIdent) {
      throw ErrorPageException();
    }
    conf->error_page.insert(std::pair<int, std::string>(http_status, CurToken()->literal));
  } else if (CurToken()->type == token_type::kClientBodyBufferSize) {
    NextToken();
    if (CurToken()->type != token_type::kInt) {
      throw CBBSException();
    }
    conf->client_body_size = stoi(CurToken()->literal);
  } else if (CurToken()->type == token_type::kIndex) {
    while (PeekToken()->type != token_type::kSemicolon) {
      NextToken();
      conf->index.insert(CurToken()->literal);
    }
  }
}

void Parser::PrintToken(Token *tok) {
  std::cout << \
    "{Type:" << tok->type << \
    " Literal:" << tok->literal << \
    "}" << std::endl;
}

const char *Parser::LbraceException::what() const throw() {
  return "left brace error";
}

const char *Parser::RbraceException::what() const throw() {
  return "right brace error";
}

const char *Parser::SemicolonException::what() const throw() {
  return "semicolon error";
}

const char *Parser::RootException::what() const throw() {
  return "root error";
}

const char *Parser::ErrorPageException::what() const throw() {
  return "error_pagh error";
}

const char *Parser::CBBSException::what() const throw() {
  return "CBBS error";
}

const char *Parser::IndexException::what() const throw() {
  return "index error";
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
