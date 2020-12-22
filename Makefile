all: prepare install 

update:
	@git pull ; git merge origin/desktop
	
prepare:
	@test -d build || ( mkdir build ; cd build ; cmake .. )
	@cd src; python valid_move.py > valid_move.h; python make_move.py > make_move.h; 
	
install:
	@cd build ; make -sj install
	@cd pyreversi; python setup.py build_ext --inplace

clean:
	@rm -rf build lib bin pyreversi/engine.*so pyreversi/build pyreversi/engine.c pyreversi/__pycache__/

server: all
	cd pyreversi; bash start_server.sh

test: all
	cd bin; ./reversi
