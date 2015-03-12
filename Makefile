all: prepare install 

update:
	@git pull ; git merge origin/desktop
	
prepare:
	@test -d build || ( mkdir build ; cd build ; cmake .. )
	@cd src; python valid_move.py > valid_move.h; python make_move.py > make_move.h; 
	
install:
	@cd build ; make -sj install

clean:
	@rm -rf build lib bin
