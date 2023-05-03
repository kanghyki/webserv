import cgi
import os

form = cgi.FieldStorage()

ret = os.environ['SESSION_AVAILABLE']

print("Content-Type: text/html\r\n\r")

print("<html>")
print("<head>")
print("</head>")
print("<body>")
print(f"<h2>{ret}</h2>")
print("</body>")
print("</html>")
