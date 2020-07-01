run: a.out
	./a.out
	#for i in `seq 1 10`; do ./a.out; done
chk: a.out
	#valgrind --tool=drd --read-var-info=yes ./a.out
	#valgrind --tool=memcheck --read-var-info=yes ./a.out
	valgrind --tool=helgrind --read-var-info=yes ./a.out
a.out: poc.c
	# gcc -g poc.c -pthread
	gcc -g var_stack_poc.c -pthread

clean:
	rm -rf a.out
