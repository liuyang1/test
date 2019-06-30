CFLAGS= -g -Wall
a.out: simple_fsm.o test_simple_fsm.o
	gcc $^ -o $@

simple_fsm.o: simple_fsm.c
	gcc $(CFLAGS) -c $< -o $@

test_simple_fsm.o: test_simple_fsm.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o a.out core vgcore.*

run: a.out
	./a.out
