a.out:func1.o main.o
	gcc $^ -o $@
func1.o:func1.c
	gcc -Wall -c $<
main.o:main.c
	gcc -Wall -c $<

