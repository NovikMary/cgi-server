#!/usr/bin/env python3


import cgi

fields = dict(cgi.FieldStorage())
reply = "It Works!\nCGI fields: {}".format(fields)
reply_size = len(reply)

print("Content-Type: text/plain")
print("Content-Length: {}".format(reply_size))
print()
print(reply)

