#ifndef WORKER_HPP
# define WORKER_HPP

# include "./config/ServerConfig.hpp"
# include "./http/HttpDataFetcher.hpp"
# include "./http/HttpRequest.hpp"
# include "./http/HttpResponse.hpp"
# include "./http/HttpResponseBuilder.hpp"

# include <vector>
# include <utility>

class Worker {
  public:
    Worker(ServerConfig config, int read_fd, int write_fd);
    ~Worker();
    void routine();

  private:
    int           read_fd;
    int           write_fd;
    ServerConfig  config;

    std::string   readPipe();
    void          writePipe(const std::string& data);
};

#endif
