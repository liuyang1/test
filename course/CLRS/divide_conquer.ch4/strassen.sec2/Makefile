mat_mul: mat_mul.c
	gcc -O2 -Wall -Werror -g -pthread mat_mul.c -o mat_mul

pg: mat_mul.c
	gcc -Wall -Werror -pg -pthread mat_mul.c -o mat_mul
	./mat_mul
	gprof mat_mul gmon.out -p

run: mat_mul
	./mat_mul

complex: complex.c
	gcc complex.c -std=c99 -lm -o complex

core:
	gdb mat_mul core

clean:
	rm -rf *.o mat_mul *.out core complex
