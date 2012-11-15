CFILE=d
SUFFIX=c
TESTSUFFIX=test

a.out: $(CFILE).$(SUFFIX)
	gcc -O2 -Wall -static -pipe $(CFILE).$(SUFFIX)
clean:
	rm -rf *.o a.out
test:
	./a.out < $(CFILE).$(TESTSUFFIX)
