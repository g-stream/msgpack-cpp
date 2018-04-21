cxx = g++-7
cflag = -std=c++17 -g -Wall

target=test
test:test.o parser.o
	${cxx}  -o $@ $^ ${cflag}

test.o:test.cpp parser.h
	${cxx}  -c ${cflag} $^
parser.o:parser.cpp parser.h
	${cxx} -c ${cflag}  $<

.PHONY:clean

clean:
	-rm test *.o

