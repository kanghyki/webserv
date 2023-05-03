#include "./Logger.hpp"

/*
 * -------------------------- Constructor --------------------------
 */

Log::Log() {
  this->level = UNKNOWN;
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
