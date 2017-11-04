SRC=brainfuck_to_c.c
run: a.out
	./a.out

check: a.out
	cppcheck $(SRC)
	valgrind ./a.out --leak-check=full

a.out: $(SRC)
	gcc $(SRC) -Wall -Werror -g

clean:
	rm -rf a.out core vgcore*
