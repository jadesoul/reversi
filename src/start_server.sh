gunicorn -w 4 server:app -b :8888 --log-level=DEBUG --debug
