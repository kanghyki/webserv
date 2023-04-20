#include "./Worker.hpp"
#include <sys/select.h>
#include <unistd.h>

Worker::Worker(ServerConfig config, int read_fd, int write_fd): config(config), read_fd(read_fd), write_fd(write_fd) {}

Worker::~Worker() {}

void Worker::routine() {
  std::cout << getNowStr() << " [notice] 1# WORKER RUN" << std::endl;
  fd_set read_set;
  struct timeval timeout;

  FD_ZERO(&read_set);
  FD_SET(this->read_fd, &read_set);

  timeout.tv_sec = 5;
  timeout.tv_usec = 0;

  while (1) {
    fd_set readsCpy = read_set;
    try {
      if (select(this->read_fd + 1, &readsCpy, NULL, NULL, NULL) != -1) {
        for (int i = 0; i < this->read_fd + 1; i++) {
          if (FD_ISSET(i, &readsCpy)) {
            std::string ret;
            std::string requestStr = readPipe();

            //////// parse----------------------------------
            HttpRequest request;
            request.parse(requestStr);
            if (request.isError()) {
              //!!!!!! parse-error----------------------------
              std::cout << "REQUEST ERROR" << std::endl;
              std::cout << request.getErrorStatus() << std::endl;

              std::string data;
              std::map<int, std::string> ep = this->config.getErrorPage();
              if (ep.find(404) != ep.end()) {
                std::string error_page_path = ep.find(404)->second;
                data = HttpDataFecther::readFile(error_page_path);
              }
              ret = HttpResponseBuilder::getBuilder()
                .statusCode(NOT_FOUND)
                .httpVersion("HTTP/1.1")
                .header("date", getNowStr())
                .body(data)
                .build()
                .toString();
            }
            //////// parse-end------------------------------
            else {
              //////// fetch----------------------------------
              HttpDataFecther fetcher(request, this->config);
              try {
                std::string data = fetcher.fetch();
                ret = HttpResponseBuilder::getBuilder()
                  .statusCode(OK)
                  .httpVersion("HTTP/1.1")
                  .header("date", getNowStr())
                  .body(data)
                  .build()
                  .toString();
              } catch (std::exception &e) {
                //!!!!!! fetch-error----------------------------
                std::cout << "FETCH ERROR" << std::endl;
                std::string data;
                std::map<int, std::string> ep = this->config.getErrorPage();
                if (ep.find(404) != ep.end()) {
                  std::string error_page_path = ep.find(404)->second;
                  data = HttpDataFecther::readFile(error_page_path);
                }
                ret = HttpResponseBuilder::getBuilder()
                  .statusCode(NOT_FOUND)
                  .httpVersion("HTTP/1.1")
                  .header("date", getNowStr())
                  .body(data)
                  .build()
                  .toString();
              }
              //////// fetch-end------------------------------
            }
            writePipe(ret);
          }
        }
      }
    } catch (std::exception &e) {
      std::cout << "WORKER ERROR # " << e.what() << std::endl;
    }
  }

  // 1. 데이터 받아오기
  // 2. request 파싱하기
  // 3. 요청 데이터 읽기 또는 실행하기
  // 4. response 만들기
  // 5. response 반환하기
}

// FIXME: NON BLOCK
# define BUF_SIZE (1000)
std::string Worker::readPipe() {
  char buf[BUF_SIZE + 1];
  size_t recv_size;
  int DONE = 0;

  recv_size = read(this->read_fd, buf, BUF_SIZE);
  buf[recv_size] = 0;
  std::string data(buf);
  return data;
}

// FIXME: NON BLOCK
void Worker::writePipe(const std::string &data) {
  write(this->write_fd, data.c_str(), strlen(data.c_str()));
}
