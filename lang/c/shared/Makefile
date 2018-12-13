CC=gcc
CFLAGS=-Wall -fPIC
LDFLAGS=-Wl,--no-undefined

all: libthird.so liba.so libb.so

liba.so: a.o
	$(CC) $(LDFLAGS) -shared -o liba.so a.o -L. -lthird

libb.so: b.o
	$(CC) $(LDFLAGS) -shared -o libb.so b.o -L. -la -lthird

a.o:a.c
	$(CC) $(CFLAGS) -c a.c -o a.o

b.o:b.c
	$(CC) $(CFLAGS) -c b.c -o b.o

libthird.so: third.o
	$(CC) $(LDFLAGS) -shared -o libthird.so third.o

third.o: third.c
	$(CC) $(CFLAGS) -c third.c -o third.o

clean:
	rm -rf *.o *.so *.bak *.xxd

bak:
	mv liba.so liba.so.bak
	mv libb.so libb.so.bak

xxd:
	xxd liba.so > liba.so.xxd
	xxd libb.so > libb.so.xxd
	xxd liba.so.bak > liba.so.bak.xxd
	xxd libb.so.bak > libb.so.bak.xxd

cmp:
	@diff liba.so liba.so.bak || echo "different"
	diff libb.so libb.so.bak
	# diff liba.so.xxd liba.so.bak.xxd
	# diff libb.so.xxd libb.so.bak.xxd
