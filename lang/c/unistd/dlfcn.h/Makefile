LIBS=-ldl
a.out: testLazyDynOpen.c
	gcc -Wall -Werror testLazyDynOpen.c $(LIBS)

run: a.out
	./a.out

clean:
	rm -rf a.out core*
