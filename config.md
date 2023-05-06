첫번째 server config가 default server가 된다.

# Common
```
1.
root [path(ident)]
default value) /html
example) root /html;

2.
error_page [HTTP status code(int) ...] [path(ident)]
default value) none
example) error_page 400 403 404 /error_page/error.html;

3.
client_body_buffer_size [size(int)]
default value) 8192
example) client_body_buffer_size 5000;

4.
index [file_name(ident)]
default value) index.html
example) index hello.html;
```

# Http
```
1.
client_header_timeout [second(int)];
default value) 60
example) client_header_timeout 50;

2.
client_body_timeout [second(int)];
default value) 60
example) client_body_timeout 50;

3.
send_timeout [second(int)];
default value) 60
example) send_timeout 50;
```

# Server
```
1.
session timeout [second(int)];
default value) 600
example) session_timeout 30;

2.
keepalive_timeout [second(int)];
default value) 60
example) keepalive_timeout 30;

3.
keepalive_requests [second(int)];
default value) 1000
example) keepalive_requests 500;

4.
listen [host(ident)]:[port(int)];
default value) 127.0.0.1:80
example) listen 192.168.0.1:4242;

5.
server_name [path(ident)];
default value) NONE
example) server_name webserv.42.kr;
```

# Location
```
1.
limit_except [HTTP method(indent) ...] ;
default value) NONE
example) limit_except GET POST DELETE;

2.
cgi [extension(indent)] [CGI path(ident, MUST BE ABSOLUTE PATH)]
default value) NONE
example) cgi .py /cgi/hello.py;

3.
autoindex [on(ident)/off(ident)]
default value) off
example) autoindex on;

4.
return [HTTP status code(int)] [URI(ident)]
default value) NONE
example) return 301 http://localhost/index.html;
```
