#!/usr/bin/python3

print("Content-Type: text/plain")
#print("Content-Length: 4")

import cgi
fields = dict(cgi.FieldStorage())

print(fields)
#print("123")
