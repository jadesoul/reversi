all: prepare install 

update:
	@git pull ; git merge origin/desktop
	
prepare:
	@test -d build || ( mkdir build ; cd build ; cmake .. )
	
install:
	@cd build ; make -sj install
	@cd src/apps/pyreversi; python setup.py build_ext --inplace

clean:
	@rm -rf build lib bin cgi-bin src/apps/pyreversi/engine.so
	
test: all
	bin/reversi

server: all
	cd src/apps/pyreversi; bash start_server.sh
