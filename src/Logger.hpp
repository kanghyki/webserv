#ifndef LOGGER_HPP
# define LOGGER_HPP

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
  DEBUG,
  UNKNOWN
};

class Log {
  public:
    Log();
    ~Log();

    static Log cout();

    Log& operator<<(logger_level s) {
      this->level = s;

      if (this->level == DEBUG && IS_DEBUG == 0)
        return *this;

      std::cout << timestamp();
      if (this->level == INFO)
        std::cout << std::string(GREEN) + "[INFO]" + std::string(RESET);
      else if (this->level == WARNING)
        std::cout << std::string(YELLOW) + "[WARNING]" + std::string(RESET);
      else if (this->level == ERROR)
        std::cout << std::string(RED) + "[ERROR]" + std::string(RESET);
      else if (this->level == DEBUG)
        std::cout << std::string(WHITE) + "[DEBUG]" + std::string(RESET);
      else
        std::cout << std::string(WHITE) + "[UNKNOWN]" + std::string(RESET);
      std::cout << " ";
      return *this;
    }

    template<typename T>
    Log& operator<<(T s) {
      if (this->level == DEBUG && IS_DEBUG == 0)
        return *this;

      if (this->level == INFO)          std::cout << std::string(BOLD_GREEN);
      else if (this->level == WARNING)  std::cout << std::string(BOLD_YELLOW);
      else if (this->level == ERROR)    std::cout << std::string(BOLD_RED);
      else if (this->level == DEBUG)    std::cout << std::string(BOLD_WHITE);
      else if (this->level == UNKNOWN)    std::cout << std::string(WHITE);
      std::cout << s << std::string(RESET);

      return *this;
    }

  private:
    logger_level level;
    std::string timestamp(void) const;
};

#endif
