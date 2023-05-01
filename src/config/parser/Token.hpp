#ifndef TOKEN_HPP
# define TOKEN_HPP

# include <string>

class Token {
  public:
    Token();
    Token(const Token& obj);
    Token(const std::string& t, const std::string& l);
    ~Token();
    Token&                    operator=(Token const& obj);

    bool                      is(const std::string& type) const;
    bool                      isNot(const std::string& type) const;
    bool                      isCommon() const;

    std::string               getType() const;
    std::string               getLiteral() const;
    size_t                    getLineNumber() const;
    size_t                    getPos() const;

    void                      setType(const std::string& type);
    void                      setLiteral(const std::string& type);
    void                      setLineNumber(size_t lineNumber);
    void                      setPos(size_t pos);

    static const std::string  ILLEGAL;
    static const std::string  END_OF_FILE;
    static const std::string  IDENT;
    static const std::string  INT;
    static const std::string  SEMICOLON;
    static const std::string  LBRACE;
    static const std::string  RBRACE;
    // Keyword
    static const std::string  HTTP;
    static const std::string  SERVER;
    static const std::string  LOCATION;
    static const std::string  LISTEN;
    static const std::string  SERVER_NAME;
    static const std::string  ROOT;
    static const std::string  ERROR_PAGE;
    static const std::string  CLIENT_BODY_BUFFER_SIZE;
    static const std::string  INDEX;
    static const std::string  ALIAS;
    static const std::string  LIMIT_EXCEPT;
    static const std::string  AUTOINDEX;
    static const std::string  RETURN;
    static const std::string  SESSION_TIMEOUT;
    static const std::string  TIMEOUT;
    static const std::string  CGI;

    enum { KEYWORD_SIZE = 16 };
    static const int          IDENT_IDX;
    static const int          TYPE_IDX;
    static const std::string  keyword[KEYWORD_SIZE][2];

  private:
    size_t      lineNumber;
    size_t      pos;
    std::string type;
    std::string literal;

};

namespace keyword {
};


#endif
