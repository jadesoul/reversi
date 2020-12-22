#!/usr/bin/python
#coding:utf8

import engine
# from engine import reversi

def app(environ, start_response):
	query = environ['QUERY_STRING'].encode('utf8')
	print('query=', query)
	data = engine.reversi(query)
	# data = b"{'next_move':'%s'}" % data
	print('result=', data)
	# print('result.type=', type(data))
	start_response("200 OK", [
		("Content-Type", "text/plain"),
		("Access-Control-Allow-Origin", "*"),
		("Content-Length", str(len(data)))
		])
	return iter([data])
