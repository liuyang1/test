CFLAGS=-DNOASM
a.out: paq8l.cpp
	g++ paq8l.cpp $(CFLAGS)

comp: a.out
	./a.out -5 1.zip ../../poc/enwik6
decomp: a.out
	more < 1.zip.paq8l | head -n 2
	./a.out -d 1.zip.paq8l
cmp:
	md5sum ../../poc/enwik6
	md5sum enwik6
