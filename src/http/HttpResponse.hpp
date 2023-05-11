#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include "./header/HttpResponseHeader.hpp"
# include "./HttpStatus.hpp"
# include "./HttpRequest.hpp"
# include "../etc/Util.hpp"

# include <map>
# include <string>

class HttpResponse {

  public:
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

    std::string                         toString() throw();

  private:
    static const std::string            version;

    HttpStatus                          statusCode;
    std::string                         statusText;
    HttpResponseHeader                  header;
    std::string                         body;

    std::string                         buffer;
    unsigned int                        buffer_size;
    unsigned int                        sendLength;

    std::string                         makeStatusLine() const;
    std::string                         getCurrentTimeStr() const;
};

// TODO: move
std::string getNowStr();

#endif
