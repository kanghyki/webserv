#ifndef HTTP_HEADER_FIELD_HPP
# define HTTP_HEADER_FIELD_HPP

# include <string>

namespace header_field {
//  const std::string CONTENT_LENGTH        =   "Content-Length";
//  const std::string CONTENT_TYPE          =   "Content-Type";
//
//  const std::string CACHE_CONTROL         =   "Cache-control";
//  const std::string HOST                  =   "Host";
//  const std::string PRAGMA                =   "Pragma";
//  const std::string RANGE                 =   "Range";
//  const std::string TE                    =   "TE";
//  const std::string EXPECT                =   "Expect";
//  const std::string MAX_FORWARDS          =   "Max-forwards";
//
//  const std::string IF_MATCH              =   "If-Match";
//  const std::string IF_NONE_MATCH         =   "If-None-Match";
//  const std::string IF_MODIFIED_SINCE     =   "If-Modified-Since";
//  const std::string IF_UNMODIFIED_SINCE   =   "If-Unmodified-Since";
//  const std::string IF_RANGE              =   "If-Range";
//
//  const std::string ACCEPT                =   "Accept";
//  const std::string ACCEPT_CHARSET        =   "Accept-Charset";
//  const std::string ACCEPT_ENCODING       =   "Accept-Encoding";
//  const std::string ACCEPT_LANGUAGE       =   "Accept-Language";
//
//  const std::string AUTHORIZATION         =   "Authorization";
//  const std::string PROXY_AUTHORIZATION   =   "Proxy-Authorization";
//
//  const std::string FROM                  =   "From";
//  const std::string REFERER               =   "Referer";
//  const std::string USER_AGENT            =   "User-Agent";

  const std::string CONTENT_LENGTH        =   "content-length";
  const std::string CONTENT_TYPE          =   "content-type";

  const std::string CACHE_CONTROL         =   "cache-control";
  const std::string HOST                  =   "host";
  const std::string PRAGMA                =   "pragma";
  const std::string RANGE                 =   "range";
  const std::string TE                    =   "te";
  const std::string EXPECT                =   "expect";
  const std::string MAX_FORWARDS          =   "max-forwards";

  const std::string IF_MATCH              =   "if-match";
  const std::string IF_NONE_MATCH         =   "if-none-match";
  const std::string IF_MODIFIED_SINCE     =   "if-modified-since";
  const std::string IF_UNMODIFIED_SINCE   =   "if-unmodified-since";
  const std::string IF_RANGE              =   "if-range";

  const std::string ACCEPT                =   "accept";
  const std::string ACCEPT_CHARSET        =   "accept-charset";
  const std::string ACCEPT_ENCODING       =   "accept-encoding";
  const std::string ACCEPT_LANGUAGE       =   "accept-language";

  const std::string AUTHORIZATION         =   "authorization";
  const std::string PROXY_AUTHORIZATION   =   "proxy-authorization";

  const std::string FROM                  =   "from";
  const std::string REFERER               =   "referer";
  const std::string USER_AGENT            =   "user-agent";
  const std::string SET_COOKIE            =   "set-cookie";
  const std::string COOKIE                =   "cookie";
  const std::string CONNECTION            =   "connection";
}

#endif
