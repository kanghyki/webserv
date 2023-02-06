#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <string>
#include <fstream>
#include <iostream>

class FileReader {
  public:
    FileReader(const std::string file_name);
    ~FileReader();

    const std::string ReadLine();
    bool IsReadable();

    class OpenFailedException: public std::exception {
      public:
        const char *what() const throw();
    };

  private:
    std::ifstream file_in;
    bool is_readable;

    FileReader();
    FileReader(const FileReader &obj);
    FileReader &operator=(const FileReader &obj);
};

#endif
