rpn: rpn.c
	gcc -Wall rpn.c -lm -o rpn

test: rpn
	./rpn < rpn.in

clean:
	rm -rf *.out core* *.log rpn
