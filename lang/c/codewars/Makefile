SRC=Egy.c
LIB=-lm
run: a.out
	./a.out

check: a.out
	cppcheck $(SRC)
	valgrind --leak-check=full ./a.out

a.out: $(SRC)
	clang $(SRC) -Wall -Werror -g $(LIB)

clean:
	rm -rf a.out core vgcore*
