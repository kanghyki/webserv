#include "./Logger.hpp"

std::string log::endl = "\n";

std::string log::timestamp() {
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

log::Error& log::Error::operator<<(std::string s) {
  if (s == log::endl) {
    std::cout << timestamp()
      << std::string(RED) + "[ERROR] " + std::string(RESET)
      << std::string(BOLD_RED) << this->buf << std::string(RESET) << std::endl;
    this->buf.clear();
  }
  else
    this->buf += s;

  return *this;
}

log::Error& log::Error::operator<<(int s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Error& log::Error::operator<<(size_t s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Error& log::Error::operator<<(unsigned int s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Error& log::Error::operator<<(short s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Error& log::Error::operator<<(unsigned short s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Warning& log::Warning::operator<<(std::string s) {
  if (s == log::endl) {
    std::cout << timestamp()
      << std::string(YELLOW) + "[WARNING] " + std::string(RESET)
      << std::string(BOLD_YELLOW) << this->buf << std::string(RESET) << std::endl;
    this->buf.clear();
  }
  else
    this->buf += s;

  return *this;
}

log::Warning& log::Warning::operator<<(int s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Warning& log::Warning::operator<<(size_t s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Warning& log::Warning::operator<<(unsigned int s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Warning& log::Warning::operator<<(short s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Info& log::Info::operator<<(std::string s) {
  if (s == log::endl) {
    std::cout << timestamp()
      << std::string(GREEN) + "[INFO] " + std::string(RESET)
      << std::string(BOLD_GREEN) << this->buf << std::string(RESET) << std::endl;
    this->buf.clear();
  }
  else
    this->buf += s;

  return *this;
}

log::Info& log::Info::operator<<(int s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Info& log::Info::operator<<(size_t s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Info& log::Info::operator<<(unsigned int s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Info& log::Info::operator<<(short s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Info& log::Info::operator<<(unsigned short s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Debug& log::Debug::operator<<(std::string s) {
  if (s == log::endl) {
    std::cout << timestamp()
      << std::string(WHITE) + "[DEBUG] " + std::string(RESET)
      << std::string(BOLD_WHITE) << this->buf << std::string(RESET) << std::endl;
    this->buf.clear();
  }
  else
    this->buf += s;

  return *this;
}

log::Debug& log::Debug::operator<<(int s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Debug& log::Debug::operator<<(size_t s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Debug& log::Debug::operator<<(unsigned int s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Debug& log::Debug::operator<<(short s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Debug& log::Debug::operator<<(unsigned short s) {
  this->buf += util::itoa(s);

  return *this;
}

log::Info     log::info;
log::Warning  log::warning;
log::Error    log::error;
log::Debug    log::debug;
