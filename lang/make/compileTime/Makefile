TT = $(shell date +"%Y-%m-%d %H:%M:%S")

a.out:a.c
	echo $(TT)
	gcc -DTT="\"$(TT)\"" a.c
run:
	./a.out
clean:
	rm a.out
