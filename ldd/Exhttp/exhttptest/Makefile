CFILE=exhttp
SUFFIX=cpp
TESTSUFFIX=test
INCLUDEPATH=/usr/local/include/libopendpi-1.3/libopendpi
LINKPATH=/usr/local/lib

a.out: exhttp.o conv.o myconv.o
	g++ -g -pg -O9 -Wall -D_debug_ -lchardet -lpcap -Wl,-rpath $(LINKPATH) -lopendpi -I$(INCLUDEPATH) exhttp.o conv.o
exhttp.o:exhttp.cpp exhttp.h
	#g++ -g -Wall -D_debug_ -c exhttp.cpp
	g++ -g -Wall -c exhttp.cpp
conv.o:conv.cpp conv.h
	g++ -g -Wall -c conv.cpp
myconv.o:myconv.c myconv.h
	g++ -g -Wall -c myconv.h
clean:
	rm -rf *.o a.out
test:
	./a.out 
