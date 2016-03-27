SRC=solveSudoku.037.c
DST=$(patsubst %.c,%.out,$(SRC))
$(info src: $(SRC) -> dst: $(DST))

$(DST):$(SRC)
	gcc $(SRC) -o $(DST) && ./$(DST)

clean:
	rm -rf *.out core core.* *.o *.pyc *.log
