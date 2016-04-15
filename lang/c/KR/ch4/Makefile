rpn: rpn.c
	gcc -Wall rpn.c -lm -o rpn

test: rpn
	./rpn < rpn.in

unit.test: rpn
	cat getint.in getfloat.in | ./rpn

clean:
	rm -rf *.out core* *.log rpn
