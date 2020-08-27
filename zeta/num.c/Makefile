CFLAGS=-Wall -g
LDFLAGS=
run: a.out
	./a.out

a.out: zint.o test_zint.o
	gcc $(LDFLAGS) $^ -o $@

zint.o: zint.c
	gcc $(CFLAGS) -c $^ -o $@

test_zint.o: test_zint.c
	gcc $(CFLAGS) -c $^ -o $@

clean:
	rm -rf *.o a.out core*
