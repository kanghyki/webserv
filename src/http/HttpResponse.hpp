#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "./CGI.hpp"
# include "./header/HttpResponseHeader.hpp"
# include "./HttpStatus.hpp"
# include "./HttpRequest.hpp"
# include "../etc/Util.hpp"

# include <map>
# include <string>

class HttpResponse {

  public:
    enum cgi_status {
      NOT_CGI,
      IS_CGI
    };

    enum SendStatus {
      SENDING,
      DONE
    };

    HttpResponse();
    ~HttpResponse();
    HttpResponse(const HttpResponse& obj);
    HttpResponse& operator=(const HttpResponse& obj);

    void                                setStatusCode(const HttpStatus statusCode);
    void                                setBody(const std::string& body);
    void                                removeBody();
    void                                addSendLength(unsigned int length);

    HttpStatus                          getStatusCode() const;
    SendStatus                          getSendStatus() const;
    HttpResponseHeader&                 getHeader();

    cgi_status                          get_cgi_status() const;
    void                                set_cgi_status(cgi_status s);

    CGI&                                getCGI();

    int                                 getStatus() const;
    int                                 getWriteFd() const;
    int                                 getReadFd() const;
    int                                 getFd() const;

    void                                openToWrite(const std::string& fileName);
    void                                openToRead(const std::string& fileName);

    void                                setAutoIndex(bool autoindex);
    bool                                isAutoindex() const;

    void                                setMethod(const std::string& method);
    std::string                         getMethod(void) const;

    std::string                         toString() throw();

  private:
    static const std::string            version;

    HttpStatus                          statusCode;
    std::string                         statusText;
    HttpResponseHeader                  header;
    std::string                         body;

    bool                                isSetBuffer;
    std::string                         buffer;
    unsigned int                        buffer_size;
    unsigned int                        sendLength;

    cgi_status                          cgi_stat;
    CGI                                 cgi;

    int                                 status;
    int                                 writeFd;
    int                                 readFd;
    int                                 fd;

    bool                                autoindex;
    std::string                         method;

    std::string                         makeStatusLine() const;
    std::string                         getCurrentTimeStr() const;
};

// TODO: move
std::string getNowStr();

#endif
