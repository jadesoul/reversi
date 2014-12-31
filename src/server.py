#!/usr/bin/python
#coding:utf8

from engine import reversi

def app(environ, start_response):
	data = "Hello, World!\n"
	start_response("200 OK", [
		("Content-Type", "text/plain"),
		("Content-Length", str(len(data)))
		])
	return iter([data])

if __name__=='__main__':
	pass

