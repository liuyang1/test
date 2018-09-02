SRC=multiplyString.c
LIB=-lm
CC=gcc
run: a.out
	./a.out

check: a.out
	cppcheck $(SRC)
	valgrind --leak-check=full ./a.out

a.out: $(SRC)
	$(CC) $(SRC) -Wall -Werror -g $(LIB)

clean:
	rm -rf a.out core vgcore*
