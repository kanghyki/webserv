#include "MimeType.hpp"

MimeType::MimeType() {
  this->mimeTypes.insert(std::pair<std::string, std::string>(".txt", "text/plain"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".html", "text/html"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".htm", "text/html"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".css", "text/css"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".js", "text/javascript"));

  this->mimeTypes.insert(std::pair<std::string, std::string>(".gif", "image/gif"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".png", "image/png"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".jpg", "image/jpg"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".jpeg", "image/jpeg"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".bmp", "image/bmp"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".webp", "image/webp"));

  this->mimeTypes.insert(std::pair<std::string, std::string>(".midi", "audio/midi"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".mpeg", "audio/mpeg"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".ogg", "audio/ogg"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".wav", "audio/wav"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".mp3", "audio/mp3"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".flac", "audio/flac"));

  this->mimeTypes.insert(std::pair<std::string, std::string>(".webm", "vedio/webm"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".avi", "vedio/avi"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".mp4", "vedio/mp4"));

  this->mimeTypes.insert(std::pair<std::string, std::string>(".exe", "application/octet-stream"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".out", "application/octet-stream"));
  this->mimeTypes.insert(std::pair<std::string, std::string>(".pdf", "application/pdf"));
}

const std::string MimeType::getMimeType(const std::string& filename) const {
  size_t pos = filename.rfind(".");

  if (pos == std::string::npos)
    return "text/plain";
  else {
    std::string extension = filename.substr(pos);
    if (this->mimeTypes.find(extension) != this->mimeTypes.end())
      return this->mimeTypes.at(extension);
  }

  return "text/plain";
}
