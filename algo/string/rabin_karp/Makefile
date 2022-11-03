perf: a.out
	time ./a.out

run: a.out
	valgrind ./a.out

a.out: rabin_karp.c
	gcc -g -Wall -O2 rabin_karp.c
	objdump -D a.out > a.asm

clean:
	rm -rf a.out vgcore*
