#!/usr/bin/python3

import cgi
import os


form = cgi.FieldStorage()

fileitem = form['file']

if fileitem.filename:
    file = os.environ['PATH_INFO'] + "/" + os.path.basename(fileitem.filename)
    if os.path.exists(file):
        message = 'File already exists'
    else:
        open(os.environ['PATH_INFO'] + "/" + file, 'wb').write(fileitem.file.read())
        message = 'The file "' + file + '" was uploaded successfully'
else:
    message = "No file was uploaded"

print("Content-Type: text/html\r\n\r")

print("<html>")
print("<head>")
print("</head>")
print("<body>")
print(f"<h2>{message}</h2>")
print("</body>")
print("</html>")
