CFILE=main
SUFFIX=cpp
TESTSUFFIX=test
INCLUDEPATH=/usr/local/include/libopendpi-1.3/libopendpi
LINKPATH=/usr/local/lib

a.out: $(CFILE).$(SUFFIX)
	g++ -O2 -Wall -lpcap -Wl,-rpath $(LINKPATH) -lopendpi -I$(INCLUDEPATH) $(CFILE).$(SUFFIX)
clean:
	rm -rf *.o a.out
test:
	./a.out 
