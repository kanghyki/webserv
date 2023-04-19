#include "Except.hpp"

namespace except {
  const char* SocketInitException::what() const throw() {
    return "Socket init failed";
  }

  const char* SocketBindException::what() const throw() {
    return "Socket bind failed";
  }

  const char* SocketListenException::what() const throw() {
    return "Socket listen failed";
  }

  const char* HttpRequestException::what() const throw() {
    return "Data not in compliance with HTTP protocol";
  }

  const char* StringFoundException::what() const throw() {
    return "Target not found";
  }

  const char* FileOpenException::what() const throw() {
    return "File open failed";
  }
}
