
all:clean test


test:
	g++  -fpermissive -std=c++11 -o test test.cpp

clean: 
	rm -f *.o
	rm -f test
