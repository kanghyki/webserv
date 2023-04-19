#ifndef EXCEPT_HPP
# define EXCEPT_HPP

# include <stdexcept>

namespace except {
  class SocketInitException : public std::exception
  {
    public:
      const char* what(void) const throw();
  };

  class SocketBindException : public std::exception
  {
    public:
      const char* what(void) const throw();
  };

  class SocketListenException : public std::exception
  {
    public:
      const char* what(void) const throw();
  };

  class HttpRequestException : public std::exception
  {
    public:
      const char* what(void) const throw();
  };

  class StringFoundException : public std::exception
  {
    public:
      const char* what(void) const throw();
  };

  class FileOpenException : public std::exception
  {
    public:
      const char* what(void) const throw();
  };
}

#endif
