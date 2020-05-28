CFLAGS=-Wall -std=c11
X=C
a.out: $(X).c
	gcc $(CFLAGS) $<

run: a.out
	./a.out < $(X).in

chk: a.out
	./a.out < $(X).in | diff $(X).out -
	valgrind ./a.out < $(X).in

clean:
	rm -rf a.out core

rel:
	cp $(X).c $(HOME)
