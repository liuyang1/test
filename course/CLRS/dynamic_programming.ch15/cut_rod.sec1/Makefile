SHELL=bash

cut_rod: cut_rod.c
	gcc -Wall -Werror -g cut_rod.c -o cut_rod

run: cut_rod
	./cut_rod

CutRod:
	ghc CutRod.hs

N=30
test: CutRod
	@for (( i=1; i<=$N; i+=1 )); do echo -n $$i ' '; done
	@echo ""
	@for (( i=1; i<=$N; i+=1 )); do ./CutRod $$i | tr '\n' ' '; done
	@echo ""

clean:
	rm -rf *.hi *.o CutRod core cut_rod vgcore.*
