gunicorn -w 1 server:app -b 127.0.0.1:8888 --log-level=DEBUG
