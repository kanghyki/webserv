#include "./Logger.hpp"

std::string logger::endl = "\n";

std::string logger::timestamp() {
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

logger::Error& logger::Error::operator<<(std::string s) {
  if (s == logger::endl) {
    std::cout << timestamp()
      << std::string(RED) + "[ERROR] " + std::string(RESET)
      << std::string(BOLD_RED) << this->buf << std::string(RESET) << std::endl;
    this->buf = "";
    this->buf.reserve();

  }
  else
    this->buf += s;

  return *this;
}

logger::Error& logger::Error::operator<<(int s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Error& logger::Error::operator<<(size_t s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Error& logger::Error::operator<<(unsigned int s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Error& logger::Error::operator<<(short s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Error& logger::Error::operator<<(unsigned short s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Warning& logger::Warning::operator<<(std::string s) {
  if (s == logger::endl) {
    std::cout << timestamp()
      << std::string(YELLOW) + "[WARNING] " + std::string(RESET)
      << std::string(BOLD_YELLOW) << this->buf << std::string(RESET) << std::endl;
    this->buf = "";
    this->buf.reserve();
  }
  else
    this->buf += s;

  return *this;
}

logger::Warning& logger::Warning::operator<<(int s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Warning& logger::Warning::operator<<(size_t s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Warning& logger::Warning::operator<<(unsigned int s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Warning& logger::Warning::operator<<(short s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Info& logger::Info::operator<<(std::string s) {
  if (s == logger::endl) {
    std::cout << timestamp()
      << std::string(GREEN) + "[INFO] " + std::string(RESET)
      << std::string(BOLD_GREEN) << this->buf << std::string(RESET) << std::endl;
    this->buf = "";
    this->buf.reserve();
  }
  else
    this->buf += s;

  return *this;
}

logger::Info& logger::Info::operator<<(int s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Info& logger::Info::operator<<(size_t s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Info& logger::Info::operator<<(unsigned int s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Info& logger::Info::operator<<(short s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Info& logger::Info::operator<<(unsigned short s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Debug& logger::Debug::operator<<(std::string s) {
  if (s == logger::endl) {
    std::cout << timestamp()
      << std::string(WHITE) + "[DEBUG] " + std::string(RESET)
      << std::string(BOLD_WHITE) << this->buf << std::string(RESET) << std::endl;
    this->buf = "";
    this->buf.reserve();
  }
  else
    this->buf += s;

  return *this;
}

logger::Debug& logger::Debug::operator<<(int s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Debug& logger::Debug::operator<<(size_t s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Debug& logger::Debug::operator<<(unsigned int s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Debug& logger::Debug::operator<<(short s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Debug& logger::Debug::operator<<(unsigned short s) {
  this->buf += util::itoa(s);

  return *this;
}

logger::Info     logger::info;
logger::Warning  logger::warning;
logger::Error    logger::error;
logger::Debug    logger::debug;
