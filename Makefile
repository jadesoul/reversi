all: prepare install 

update:
	@git pull ; git merge origin/desktop
	
prepare:
	@test -d build || ( mkdir build ; cd build ; cmake .. )
	
install:
	@cd build ; make -sj install

clean:
	@rm -rf build lib bin
