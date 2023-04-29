#!/usr/bin/python3

import cgi
import os

form = cgi.FieldStorage()

rootPath = os.environ['PATH_TRANSLATED'].replace(os.environ['SCRIPT_NAME'], "")
filePath = rootPath + "/database/user"
id = form.getvalue('id')
pw = form.getvalue('pw')

if not os.path.exists(filePath):
    print("Login failed")
    exit(0)

f = open(filePath, 'r')

for line in f:
    s = list(line.strip().split('|'))
    if len(s) == 3 and s[0] == id and s[1] == pw:
        print("Login success")
        exit(1)

f.close()
print("Login failed")
exit(0);
