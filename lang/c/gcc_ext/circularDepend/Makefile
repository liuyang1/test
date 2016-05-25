CC=gcc

CFLAGS=-Wall -fPIC

# When enable this option, build fail as
# undefined reference to `func_x`

# Toggle below line
# TEST=1
ifdef TEST
LDFLAGS=-Wl,--no-undefined
else
LDFLAGS=
endif

all:liba.so libb.so a.out

liba.so: a.o
	$(CC) $(LDFLAGS) -shared -o liba.so a.o

libb.so: b.o
	$(CC) $(LDFLAGS) -shared -o libb.so b.o

a.o:a.c
	$(CC) $(CFLAGS) -c a.c -o a.o

b.o:b.c
	$(CC) $(CFLAGS) -c b.c -o b.o

main.o:main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

a.out:main.o liba.so libb.so
	$(CC) $(LDFLAGS) main.o -o a.out -L. -la -lb

ldd:
	ldd a.out

run:
	export LD_LIBRARY_PATH=. && ./a.out

clean:
	rm -rf *.o lib*.so.1.0.1 lib*.so a.out
