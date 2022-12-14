main.o: main.c
	gcc -c main.c -o main.o

lib.o: lib.c
	gcc -c lib.c -o lib.o

wrap.o: wrap.c
	gcc -c wrap.c -o wrap.o

real: main.o lib.o
	gcc main.o lib.o -o real

wrap: lib.o wrap.o main.o
	objcopy --redefine-sym func=real_func lib.o
	gcc lib.o wrap.o main.o -o wrap

clean:
	rm -rf *.o real wrap
