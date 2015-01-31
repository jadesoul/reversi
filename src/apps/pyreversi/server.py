#!/usr/bin/python
#coding:utf8

from engine import reversi

def app(environ, start_response):
	data = reversi(environ['QUERY_STRING'])
	start_response("200 OK", [
		("Content-Type", "text/plain"),
		("Content-Length", str(len(data)))
		])
	return iter([data])





