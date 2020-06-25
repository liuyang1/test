
run: a.out
	./a.out
chk: a.out
	valgrind ./a.out
a.out: poc.c
	gcc poc.c -lrt -pthread
