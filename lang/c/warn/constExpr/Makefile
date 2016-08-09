all: p.out a.out

a.out: constExpr.c
	gcc constExpr.c

p.out: constExpr.c
	g++ constExpr.c -o p.out

clean:
	rm -rf *.out
