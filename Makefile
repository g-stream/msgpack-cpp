cxx = clang
cflag =  -g -Wall -std=c++14

target=test

test:test.cpp msgpack.h
	${cxx}  -o $@ $< ${cflag}
.PHONY:clean run

clean:
	-rm test *.o
run:
	./test
