#include "./HttpResponseBuilder.hpp"

HttpResponseBuilder::HttpResponseBuilder() {}

HttpResponseBuilder::~HttpResponseBuilder() {}

HttpResponseBuilder HttpResponseBuilder::getBuilder() {
  return HttpResponseBuilder();
}

HttpResponseBuilder& HttpResponseBuilder::statusCode(const HttpStatus code) {
  this->response.setStatusCode(code);

  return *this;
}

HttpResponseBuilder& HttpResponseBuilder::header(const std::string &key, const std::string &value) {
  this->response.setHeader(key, value);

  return *this;
}

//HttpResponseBuilder& HttpResponseBuilder::body(const std::string& body) {
//  this->response.setBody(body);
//
//  return *this;
//}

HttpResponseBuilder& HttpResponseBuilder::body(const std::string& body, const std::string& mimeType) {
  this->response.setBody(body);
  this->response.setHeader(header_field::CONTENT_TYPE, mimeType);

  return *this;
}

HttpResponse HttpResponseBuilder::build() {
  return this->response;
}
