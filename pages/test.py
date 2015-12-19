#!/usr/bin/python3

print("Content-Type: text/plain")
print()

import cgi
fields = dict(cgi.FieldStorage())

print(fields)
