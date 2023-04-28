#ifndef HTTP_RESPONSE_TEST_HPP
# define HTTP_RESPONSE_TEST_HPP

# include "../test_lib/kunit.hpp"
# include "../../src/http/HttpResponseBuilder.hpp"
# include "../../src/http/HttpResponse.hpp"
# include "../../src/http/HttpStatus.hpp"

class HttpResponseTest {
  public:
    static void response_toString_test();
    static void response_toString_test2();
  private:
};

#endif
