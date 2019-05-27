# CFLAGS=-O3
# CFLAGS=

a.out: func.o main.o
	gcc ${CFLAGS} -o $@ $^

func.o: func.c
	gcc ${CFLAGS} -c $^

main.o: main.c
	gcc ${CFLAGS} -c $^

run: a.out
	./a.out

clean:
	rm -rf *.o a.out
