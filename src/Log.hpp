#ifndef LOG_HPP
# define LOG_HPP

# include <iostream>

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

enum logger_level {
  INFO,
  WARNING,
  ERROR,
  DEBUG
};

class Log {
  public:
    Log();
    ~Log();

    static bool is_debug;
    static Log cout();

    Log& operator<<(logger_level s);
    Log& operator<<(const std::string& s);

  private:
    logger_level level;
    std::string timestamp(void) const;
};

#endif
