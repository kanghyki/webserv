#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <iostream>
# include "./Util.hpp"

# define RESET "\033[0;0m"
# define CYAN "\e[0;36m"          // TIMESTAMP

# define RED "\e[0;31m"           // ERROR
# define GREEN "\e[0;32m"         // INFO
# define YELLOW "\e[0;33m"        // WARNING
# define WHITE "\e[0;37m"         // DEBUG

enum LOG_LEVEL {
  DEBUG,
  ERROR,
  WARNING,
  INFO
};

# define LOG_LEVEL_DEBUG (0)
# define LOG_LEVEL_ERROR (1)
# define LOG_LEVEL_WARNING (2)
# define LOG_LEVEL_INFO (3)

class Logger {
  private:
    Logger& operator=(const Logger& logger);
    Logger(const Logger& logger);
    std::string buf;
    std::string prefix;
    std::string suffix;

    std::string timestamp();

  protected:

  public:
    explicit Logger(LOG_LEVEL level);
    ~Logger();

    Logger& operator<<(std::string s);
    Logger& operator<<(int s);
    Logger& operator<<(size_t s);
    Logger& operator<<(unsigned int s);
    Logger& operator<<(short s);
    Logger& operator<<(unsigned short s);
};

namespace logger {
  extern Logger info;
  extern Logger error;
  extern Logger warning;
  extern Logger debug;
  extern std::string endl;
};

#endif
