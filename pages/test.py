#!/usr/bin/env python3

print("Content-Type: text/plain")
print("Content-Length: 4")
print()
import cgi
fields = dict(cgi.FieldStorage())

#print(fields)

print("123")
