#include "server.hpp"

Server::Server(std::string host, int port) : socket(host, port) {
  std::cout << "hello webserv!!" << std::endl;
}

void Server::start(void) {
  fd_set reads_cpy;
	struct timeval timeout;
  int fd_num;
	socklen_t adr_sz;	
  int clnt_sock;
	struct sockaddr_in clnt_addr;

	while(1)
	{
		reads_cpy = socket.GetReads();
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if ((fd_num = select(socket.GetFdMax() + 1, &reads_cpy, 0, 0, &timeout)) == -1)
			break;
		if (fd_num == 0)
			continue;

		for (int i = 0; i < socket.GetFdMax() + 1; i++)
		{
			if (FD_ISSET(i, &reads_cpy))
			{
				if (i == socket.GetServSock())
				{
					adr_sz = sizeof(clnt_addr);
					clnt_sock = accept(socket.GetServSock(), (struct sockaddr*)&clnt_addr, &adr_sz);
					FD_SET(clnt_sock, &socket.GetReads());
					if (socket.GetFdMax() < clnt_sock)
            socket.SetFdMax(clnt_sock);
					const char * ddd = "\
HTTP/1.1 200 OK\r\n\
Server: Hyeongki&Kanghyki server\r\n\
Content-Length: 63\r\n\
Content-Type: text/html\r\n\
\r\n\
<!doctype html>\
<html>\
<body>\
<h1>Hello, World!</h1>\
</body>\
</html>";
					write(clnt_sock, ddd, strlen(ddd));
					close(clnt_sock);
					FD_CLR(clnt_sock, &socket.GetReads());
					printf("connected client: %d \n", clnt_sock);
				}
				else
				{
          char buf[BUF_SIZE];
					int str_len = read(i, buf, BUF_SIZE);
					if (str_len == 0)
					{
						FD_CLR(i, &socket.GetReads());
						close(i);
						printf("closed client: %d \n", i);
					}
					else
					{
						printf("buf: %s", buf);
					}
				}
			}
		}
	}
	close(socket.GetServSock());
}
