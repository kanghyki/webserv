#!/usr/bin/python3

import cgi
import os

def createDirectory(directory):
    try:
        if not os.path.exists(directory):
            os.makedirs(directory)
    except OSError:
        print("Error: Failed to create the directory.")

form = cgi.FieldStorage()

fileitem = form['file']

if fileitem.filename:
    rootPath = os.environ['PATH_TRANSLATED'].replace(os.environ['SCRIPT_NAME'], "")
    uploadPath =  rootPath + "/upload" + os.environ['PATH_INFO']
    file = os.path.basename(fileitem.filename)
    filePath = uploadPath + "/" + file
    createDirectory(uploadPath)
    if os.path.exists(filePath):
        message = 'File already exists'
    else:
        open(filePath, 'wb').write(fileitem.file.read())
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
