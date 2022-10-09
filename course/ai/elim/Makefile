.PHONY: bench sample clean zip
bench: a.out
	# valgrind --leak-check=full ./a.out bench
	time ./a.out bench

sample: a.out
	rm -rf *.txt
	./a.out sample
	# valgrind --leak-check=full ./a.out sample
	zip 1.zip -r *.txt
	mv 1.zip ~/

clean:
	rm -rf a.out *.txt vgcore.* gmon.out

a.out: elim.c
	gcc -g -Wall -Werror elim.c

prof:
	gcc -g -Wall -Werror -pg elim.c
	rm -rf *.txt
	./a.out sample
	./a.out sample
	gprof -b a.out gmon.out > report.txt
	head -n 20 report.txt
