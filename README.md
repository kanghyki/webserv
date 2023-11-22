# webserv
A handmade web server.

### Usage
> default.conf must exist, if you launched webserv without a configuration path

```shell
make && ./webserv (configuration-path)
```

### Configuration

##### Common

Properties that all config blocks have.

```
root [path(ident)]
default value) /html
example) root /html;
```

```
error_page [HTTP status code(int) ...] [path(ident)]
default value) NONE
example) error_page 400 403 404 /error_page/error.html;
```

```
client_max_body_size [size(int)]
default value) 8192
example) client_max_body_size 5000;
```

```
index [file_name(ident)]
default value) index.html
example) index hello.html;
```

##### Http

```
client_header_timeout [second(int)];
default value) 60
example) client_header_timeout 50;
```

```
client_body_timeout [second(int)];
default value) 60
example) client_body_timeout 50;
```

```
send_timeout [second(int)];
default value) 60
example) send_timeout 50;
```

##### Server

```
listen [host(ident)]:[port(int)];
default value) 127.0.0.1:80
example) listen 127.0.0.1:4242;
```

```
server_name [path(ident)];
default value) ""
example) server_name webserv.42.kr;
```

```
keepalive_timeout [second(int)];
default value) 60
example) keepalive_timeout 30;
```

```
keepalive_requests [second(int)];
default value) 1000
example) keepalive_requests 500;
```

```
gateway_timeout [second(int)];
default value) 60
example) gateway_timeout 30;
```

```
session_timeout [second(int)];
default value) 600
example) session_timeout 30;
```

##### Location

```
limit_except [HTTP method(ident) ...] ;
default value) ["GET", "HEAD", "POST", "DELETE", "PUT"]
example) limit_except GET POST DELETE;
```

```
cgi [extension(ident)] [CGI path(ident, MUST BE ABSOLUTE PATH)]
default value) NONE
example) cgi .py /cgi/hello.py;
```

```
autoindex [on(ident)/off(ident)]
default value) off
example) autoindex on;
```

```
return [HTTP status code(int)] [URI(ident)]
default value) -1, ""
example) return 301 http://localhost/index.html;
```

```
alias [path(ident)]
default value) ""
example) alias /alias_path;
```
