CFILE=exhttp
SUFFIX=cpp
TESTSUFFIX=test
INCLUDEPATH=/usr/local/include/libopendpi-1.3/libopendpi
LINKPATH=/usr/local/lib

a.out:  	myconv.o test.o
	g++ -g -O2 -Wall myconv.o test.o
test.o: 	test.cpp
	g++ -g -O2 -Wall -c test.cpp
myconv.o:	myconv.cpp myconv.h
	g++ -g -O2 -Wall -c myconv.cpp
clean:
	rm -rf *.o a.out
test:
	./a.out 
