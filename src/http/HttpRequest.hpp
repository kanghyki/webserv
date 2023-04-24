/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 18:33:07 by kanghyki          #+#    #+#             */
/*   Updated: 2023/04/24 18:47:38 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include "./HttpStatus.hpp"
# include "../Util.hpp"

# include <stdexcept>
# include <string>
# include <vector>
# include <map>

namespace request_method {
  const std::string GET                   =   "GET";
  const std::string POST                  =   "POST";
  const std::string DELETE                =   "DELETE";
}

namespace request_field {
  const std::string CACHE_CONTROL         =   "Cache-control";
  const std::string HOST                  =   "Host";
  const std::string PRAGMA                =   "Pragma";
  const std::string RANGE                 =   "Range";
  const std::string TE                    =   "TE";
  const std::string EXPECT                =   "Expect";
  const std::string MAX_FORWARDS          =   "Max-forwards";

  const std::string IF_MATCH              =   "If-Match";
  const std::string IF_NONE_MATCH         =   "If-None-Match";
  const std::string IF_MODIFIED_SINCE     =   "If-Modified-Since";
  const std::string IF_UNMODIFIED_SINCE   =   "If-Unmodified-Since";
  const std::string IF_RANGE              =   "If-Range";

  const std::string ACCEPT                =   "Accept";
  const std::string ACCEPT_CHARSET        =   "Accept-Charset";
  const std::string ACCEPT_ENCODING       =   "Accept-Encoding";
  const std::string ACCEPT_LANGUAGE       =   "Accept-Language";

  const std::string AUTHORIZATION         =   "Authorization";
  const std::string PROXY_AUTHORIZATION   =   "Proxy-Authorization";

  const std::string FROM                  =   "From";
  const std::string REFERER               =   "Referer";
  const std::string USER_AGENT            =   "User-Agent";
}

class HttpRequest {
  public:
    HttpRequest();
//    HttpRequest(std::string request);
    ~HttpRequest();

    void                                parseHeader(const std::string &h);

    std::string                         getMethod() const;
    std::string                         getPath() const;
    std::string                         getVersion() const;
    std::string                         getField(const std::string& field) const;
    std::string                         getBody() const;

    void                                setBody(const std::string& body);

  private:
    static const size_t                 URL_MAX_LENGTH;
    static const std::string            CRLF;

    std::string                         method;
    std::string                         path;
    std::string                         version;
    std::map<std::string, std::string>  field;
    std::string                         body;

    void                                parseStatusLine(const std::string &line);

    void                                validateMethod(const std::string &method);
    void                                validateVersion(const std::string &path);
    void                                validatePath(const std::string &version);

    std::pair<std::string, std::string> splitField(const std::string& line);

//    std::map<std::string, void (*)(const std::string &)> fieldMethod;

//    static void parseCacheControl(const std::string &s);
//    static void parseHost(const std::string &s);
//    static void parsePragma(const std::string &s);
//    static void parseRange(const std::string &s);
//    static void parseTE(const std::string &s);
//    static void parseExpect(const std::string &s);
//    static void parseMaxForwards(const std::string &s);
//    static void parseIfMatch(const std::string &s);
//    static void parseIfNoneMatch(const std::string &s);
//    static void parseIfModifiedSince(const std::string &s);
//    static void parseIfUnmodifiedSince(const std::string &s);
//    static void parseIfRange(const std::string &s);
//
//    static void parseAccept(const std::string &s);
//    static void parseAcceptCharset(const std::string &s);
//    static void parseAcceptEncoding(const std::string &s);
//    static void parseAcceptLanguage(const std::string &s);
//
//    static void parseAuthorization(const std::string &s);
//    static void parseProxyAuthorization(const std::string &s);
//    static void parseFrom(const std::string &s);
//    static void parseReferer(const std::string &s);
//    static void parseUserAgent(const std::string &s);
};

#endif
