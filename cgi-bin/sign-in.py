#!/usr/bin/python3

import cgi
import os
import random
import string

def generate_session_id():
    return ''.join(random.choices(string.ascii_letters + string.digits, k=16))

def return_ret(ret):
    print("Set-Cookie: _webserv_session=" + generate_session_id() + "; Path=/\r")
    print("Content-Type: text/html\r\n\r")

    print("<html>")
    print("<head>")
    print("</head>")
    print("<body>")
    print('<script type="text/javascript">')
    if ret == True:
        print("window.location.href='/';")
    else:
        print('alert("sing-in failed")')
        print("window.location.href='/html/sign-in.html';")
    print("</script")
    print(f"<h2>{ret}</h2>")
    print("</body>")
    print("</html>")
    exit(0);


form = cgi.FieldStorage()

rootPath = os.environ['PATH_TRANSLATED'].replace(os.environ['SCRIPT_NAME'], "")
filePath = rootPath + "/database/user"
id = form.getvalue('id')
pw = form.getvalue('pw')

if not os.path.exists(filePath):
    ret = False
    return_ret(ret);

f = open(filePath, 'r')
ret = False

for line in f:
    s = list(line.strip().split('|'))
    if len(s) == 3 and s[0] == id and s[1] == pw:
        ret = True
        break
f.close()

return_ret(ret);
