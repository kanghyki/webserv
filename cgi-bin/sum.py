#!/usr/bin/python3
import cgi
form = cgi.FieldStorage()

name = form.getvalue("name")
a = form.getvalue('a')
b = form.getvalue('b')

print("<html>")
print("<head>")
print("<title> hello cgi </title>")
print("</head>")
print("<body>")
print("<h1>Our simple calculator</h1>")
print("<h2>" + a + "+" + b + " : " + str(int(a) + int(b)) + "</h2>")
print("</body>")
print("</html>")
