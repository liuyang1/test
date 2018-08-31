mat_mul: mat_mul.c
	gcc -Wall -Werror -g mat_mul.c -o mat_mul

run: mat_mul
	./mat_mul

core:
	gdb mat_mul core

clean:
	rm -rf *.o mat_mul
