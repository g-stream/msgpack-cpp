cxx = clang++-7
cflag =  -g -Wall

target=test

test:test.cpp msgpack.h
	${cxx}  -o $@ $< ${cflag}
.PHONY:clean run

clean:
	-rm test *.o
run:
	./test
