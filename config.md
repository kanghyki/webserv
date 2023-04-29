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

# Server
```
1.
session timeout [second(int)];
default value) 600
example) session_timeout 30;

2.
timeout [second(int)];
default value) 60
example) timeout 35;

3.
listen [host(ident)]:[port(int)];
default value) 127.0.0.1:80
example) listen 192.168.0.1:4242;

4.
server_name [path(ident)];
default value) NONE
example) server_name webserv.42.kr;
```

# Location
```
1.
alias [path(ident)];
default value) NONE
example) alias /secret_html/index.html;

2.
limit_except [HTTP method(indent) ...] ;
default value) NONE
example) limit_except GET POST DELETE;

3.
cgi [extension(indent)] [CGI path(ident)]
default value) NONE
example) cgi .py /cgi/hello.py;

4.
autoindex [on(ident)/off(ident)]
default value) off
example) autoindex on;

5.
return [HTTP status code(int)] [URI(ident)]
default value) NONE
example) return 301 http://localhost/index.html;
```