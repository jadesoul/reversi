all: prepare install

update:
	@git pull ; git merge origin/desktop
	
prepare:
	@test -d build || ( mkdir build ; cd build ; cmake ../src )
	
install:
	@cd build ; make -sj install
	@cd src; python setup.py build_ext --inplace

clean:
	@rm -rf build lib bin cgi-bin
	
test: all
	bin/reversi
