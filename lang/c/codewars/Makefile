run: a.out
	./a.out

check: a.out poohbear.c
	cppcheck poohbear.c
	valgrind ./a.out

a.out: poohbear.c
	gcc poohbear.c -Wall -Werror

clean:
	rm -rf a.out
