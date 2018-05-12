cxx = g++-7
cflag =  -g -Wall

target=test

test:test.cpp msgpack.h testharness.h
	${cxx}  -o $@ $< ${cflag}
.PHONY:clean

clean:
	-rm test *.o

