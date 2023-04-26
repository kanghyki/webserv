#!/usr/bin/python3
import cgi
form = cgi.FieldStorage()

a = form.getvalue('a')
b = form.getvalue('b')

print('Content-Type: text/plain\r\n')
print(int(a) + int(b))
