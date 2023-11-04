#include "./Logger.hpp"

Logger logger::info(INFO);
Logger logger::warning(WARNING);
Logger logger::error(ERROR);
Logger logger::debug(DEBUG);
std::string logger::endl = "\n";

std::string Logger::timestamp() {
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

Logger::Logger(LOG_LEVEL level) {
  switch (level) {
    case DEBUG:
      this->prefix = std::string(WHITE) + "[DEBUG] ";
      break;
    case ERROR:
      this->prefix = std::string(RED) + "[ERROR] ";
      break;
    case WARNING:
      this->prefix = std::string(YELLOW) + "[WARNING] ";
      break;
    case INFO:
      this->prefix = std::string(GREEN) + "[INFO] ";
      break;
  }
  this->suffix = std::string(RESET);
}

Logger::~Logger() {}

Logger& Logger::operator<<(int s) {
  this->buf += util::itoa(s);

  return *this;
}

Logger& Logger::operator<<(size_t s) {
  this->buf += util::itoa(s);

  return *this;
}

Logger& Logger::operator<<(unsigned int s) {
  this->buf += util::itoa(s);

  return *this;
}

Logger& Logger::operator<<(short s) {
  this->buf += util::itoa(s);

  return *this;
}

Logger& Logger::operator<<(unsigned short s) {
  this->buf += util::itoa(s);

  return *this;
}

Logger& Logger::operator<<(std::string s) {
  if (s == logger::endl) {
    std::cout << this->timestamp() << this->prefix << this->buf << RESET << std::endl;
    this->buf = "";
    this->buf.reserve();
  }
  else this->buf += s;

  return *this;
}
