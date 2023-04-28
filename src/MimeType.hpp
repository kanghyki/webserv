#ifndef MIME_TYPE_HPP
# define MIME_TYPE_HPP

# include <map>
# include <string>

class MimeType {
  public:
    MimeType();
    const std::string getMimeType(const std::string& filename) const;

  private:
    std::map<std::string, std::string> mimeTypes;
};

#endif
