run: test
	xt-run ./test

test: incbin.o test.o
	xt-xcc incbin.o test.o -o test -mlsp=/home/yang.liu/T/sim

incbin.o: incbin.c
	cpp incbin.c > 1.c
	xt-xcc -c incbin.c -o incbin.o
	xt-objdump -x incbin.o

test.o: test.c
	xt-xcc -c test.c -o test.o

clean:
	rm -rf *.o test
