CFILE=b
SUFFIX=cpp
TESTSUFFIX=test
CC=g++

a.out: $(CFILE).$(SUFFIX)
	$(CC) -O2 -Wall -static -pipe $(CFILE).$(SUFFIX)
clean:
	rm -rf *.o a.out
test:
	./a.out < $(CFILE).$(TESTSUFFIX)
