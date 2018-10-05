PROBLEM=B
run: a.out
	./a.out < $(PROBLEM).input
correct: a.out
	./a.out < $(PROBLEM).input
	./a.out < rand.10.C.input
a.out: $(PROBLEM).c
	gcc $(PROBLEM).c -Wall
check: a.out
	cppcheck $(PROBLEM).c
	valgrind ./a.out < $(PROBLEM).input
clean:
	rm -rf core vgcore* a.out
