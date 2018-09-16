a.out: sort.c
	gcc -g -Wall sort.c

check: a.out
	valgrind ./a.out

run: a.out
	./a.out

clean:
	rm -rf a.out core vgcore.*
