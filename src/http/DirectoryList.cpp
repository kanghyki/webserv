#include "./DirectoryList.hpp"

std::string DirectoryList::generate(const HttpRequest& req) throw(HttpStatus) {
  std::string     ret;
  std::string     dirpath = "." + req.getPath();
  DIR*            directory = opendir(dirpath.c_str());
  struct dirent*  entry;

  if (directory == NULL) throw NOT_FOUND;
  ret = \
"<html>\n\
  <head>\n\
    <title>Index of " + req.getPath() + "</title>\n\
  </head>\n\
  <body>\n\
    <h1>Index of " + req.getPath() + "</h1>\n\
  <table>\n\
  <tr><th align=\"left\">Name</th><th>Type</th><th>Size</th></tr>\
\n\
";

  while ((entry = readdir(directory)) != NULL) {
    ret += "<tr><td><a href=\"";
    if (std::string(entry->d_name) == ".") ret += req.getPath();
    else if (req.getPath() != "/") ret += req.getPath() + "/" + entry->d_name;
    else ret += req.getPath() + entry->d_name;
    ret += "\">";
    ret += entry->d_name;
    ret += "<a></td>\n";


    ret += "<td align=\"right\">";
    if (entry->d_type == DT_DIR) ret += "Directory";
    else if (entry->d_type == DT_REG) ret += "File";
    ret += "</td><td align=\"right\">"
      + util::itoa(entry->d_reclen)
      + "byte</td></tr>\n";
  }
  ret += "</table>\n\
</body>\n\
</html>\n";

  closedir (directory);

  return ret;
}
