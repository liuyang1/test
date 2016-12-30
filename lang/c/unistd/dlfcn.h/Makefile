LIBS=-ldl
a.out: testDynOpen.c
	gcc testDynOpen.c $(LIBS)

run: a.out
	./a.out

clean:
	rm -rf a.out core*
