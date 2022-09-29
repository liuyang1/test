.PHONY: bench sample clean zip
bench: a.out
	# valgrind --leak-check=full ./a.out bench
	./a.out bench

sample: a.out
	rm -rf *.txt
	valgrind --leak-check=full ./a.out sample
	zip 1.zip -r *.txt
	mv 1.zip ~/

clean:
	rm -rf a.out *.txt vgcore.*

a.out: elim.c
	gcc -g -Wall -Werror elim.c
