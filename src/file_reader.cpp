#include "file_reader.hpp"

FileReader::FileReader(const std::string file_name): file_in(file_name), is_readable(true) {
  if (!this->file_in.is_open())
    throw OpenFailedException();
}

FileReader::~FileReader() {
  file_in.close();
}

const std::string FileReader::ReadLine() {
  std::string line;
  if (std::getline(file_in, line).eof())
    this->is_readable = false;
  return line;
}

const std::string FileReader::Read() {
  std::string ret;
  while (this->is_readable)
    ret += ReadLine();
  return ret;
}

bool FileReader::IsReadable() {
  return this->is_readable;
}

const char *FileReader::OpenFailedException::what() const throw() {
  return "Unable to open";
}
