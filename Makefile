cxx = g++-7
cflag = -std=c++14 -g -Wall

target=test

test:test.cpp testharness.o config.o parser.o
	${cxx} ${cflag} -o $@ $<

testharness.o:testharness.h
	${cxx} ${cflag} -o $@ $<
config.o:config.h 
	${cxx} ${cflag} -o $@ $<
parser.o:parser.h 
	${cxx} ${cflag} -o $@ $<
.PHONY:clean

clean:
	-rm test *.o
