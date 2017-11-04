SRC=largeFactorial.c
run: a.out
	./a.out

check: a.out $(SRC)
	cppcheck $(SRC)
	valgrind ./a.out --leak-check=full

a.out: $(SRC)
	gcc $(SRC) -Wall -Werror

clean:
	rm -rf a.out
