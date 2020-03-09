run: test
	./test

test: test.o hello.o
	gcc test.o hello.o -o test

hello.o: hello
	ld -r -b binary hello -o hello.o
	objdump -x hello.o
	objdump -D hello.o > hello.asm

test.o: test.c
	gcc -c test.c -o test.o

clean:
	rm -rf *.o test *.asm core*
