#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <iostream>
# include "./Util.hpp"

# define RESET "\033[0;0m"
# define CYAN "\e[0;36m"    // TIMESTAMP

# define RED "\e[0;31m"     // ERROR
# define GREEN "\e[0;32m"   // INFO
# define YELLOW "\e[0;33m"  // WARNING
# define WHITE "\e[0;37m"   // DEBUG

# define BOLD_RED "\e[1;31m"        // ERROR
# define BOLD_GREEN "\e[1;32m"      // INFO
# define BOLD_YELLOW "\e[1;33m"  // WARNING
# define BOLD_WHITE "\e[1;37m"      // DEBUG

namespace logger {

  extern std::string endl;
  std::string timestamp();

  class Error {
    public:
      Error& operator<<(std::string s);
      Error& operator<<(int s);
      Error& operator<<(size_t s);
      Error& operator<<(unsigned int s);
      Error& operator<<(short s);
      Error& operator<<(unsigned short s);

    private:
      std::string buf;
  };
  class Warning {
    public:
      Warning& operator<<(std::string s);
      Warning& operator<<(int s);
      Warning& operator<<(size_t s);
      Warning& operator<<(unsigned int s);
      Warning& operator<<(short s);
      Warning& operator<<(unsigned short s);

    private:
      std::string buf;
  };
  class Info {
    public:
      Info& operator<<(std::string s);
      Info& operator<<(int s);
      Info& operator<<(size_t s);
      Info& operator<<(unsigned int s);
      Info& operator<<(short s);
      Info& operator<<(unsigned short s);

    private:
      std::string buf;
  };
  class Debug {
    public:
      Debug& operator<<(std::string s);
      Debug& operator<<(int s);
      Debug& operator<<(size_t s);
      Debug& operator<<(unsigned int s);
      Debug& operator<<(short s);
      Debug& operator<<(unsigned short s);

    private:
      std::string buf;
  };

  extern Info info;
  extern Error error;
  extern Warning warning;
  extern Debug debug;

};


#endif
