import cgi
import os

def createDirectory(directory):
    try:
        if not os.path.exists(directory):
            os.makedirs(directory)
    except OSError:
        print("Error: Failed to create the directory.")


form = cgi.FieldStorage()

rootPath = os.environ['PATH_TRANSLATED'].replace(os.environ['SCRIPT_NAME'], "")
dbPath = rootPath + "/database"
createDirectory(dbPath);
dbFile = dbPath + "/user"

open(dbFile, 'a').write(form.getvalue('id') + "|" + form.getvalue("password") + "|" + form.getvalue("email") + "\n")

print("Content-Type: text/html\r\n\r")

print("<html>")
print("<head>")
print("</head>")
print("<body>")
print("<h2>Congratulations</h2>")
print("</body>")
print("</html>")
