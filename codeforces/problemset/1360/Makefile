CFLAGS=-Wall -std=c11
X=B
a.out: $(X).c
	gcc $(CFLAGS) $<

run: a.out
	./a.out < $(X).in | diff $(X).out -

clean:
	rm -rf a.out core

rel:
	cp $(X).c $(HOME)
