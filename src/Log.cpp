#include "Log.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Log::Log() {
//  this->is_debug = false;
}

/*
 * -------------------------- Destructor ---------------------------
 */

Log::~Log() {
}

/*
 * -------------------------- Operator -----------------------------
 */

/*
 * -------------------------- Getter -------------------------------
 */

/*
 * -------------------------- Setter -------------------------------
 */

/*
 * ----------------------- Member Function -------------------------
 */

Log Log::cout() {
  return Log();
}

Log& Log::operator<<(logger_level s) {
  this->level = s;

//  if (this->level == DEBUG && !this->is_debug) {
//    return *this;
//  }

  std::cout << timestamp();
  if (this->level == INFO)
    std::cout << std::string(GREEN) + "[INFO]" + std::string(RESET);
  else if (this->level == WARNING)
    std::cout << std::string(YELLOW) + "[WARNING]" + std::string(RESET);
  else if (this->level == ERROR)
    std::cout << std::string(RED) + "[ERROR]" + std::string(RESET);
  else if (this->level == DEBUG)
    std::cout << std::string(WHITE) + "[DEBUG]" + std::string(RESET);
  return *this;
}

Log& Log::operator<<(const std::string& s) {
//  if (this->level == DEBUG && !this->is_debug) {
//    return *this;
//  }

  std::cout << " ";
  if (this->level == INFO)          std::cout << std::string(BOLD_GREEN);
  else if (this->level == WARNING)  std::cout << std::string(BOLD_YELLOW);
  else if (this->level == ERROR)    std::cout << std::string(BOLD_RED);
  else if (this->level == DEBUG)    std::cout << std::string(BOLD_WHITE);
  std::cout << s << std::string(RESET) << std::endl;

  this->level = DEBUG;

  return *this;
}

std::string Log::timestamp() const {
  std::string ret;
  time_t      cur_time;
  char        buf[100];

  cur_time = time(NULL);
  tm *time = localtime(&cur_time);
  strftime(buf, 100, "%d/%b/%Y:%X %Z", time);

  ret = buf;
  ret = std::string(CYAN) + "[" + ret + "]" + std::string(RESET);

  return ret;
}

/*
 * ---------------------- Non-Member Function ----------------------
 */
