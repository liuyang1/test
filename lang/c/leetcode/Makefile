SRC=solveSudoku.037.c
DST=$(patsubst %.c,%.out,$(SRC))
CFLAGS=-Wall
$(info src: $(SRC) -> dst: $(DST))

$(DST):$(SRC)
	gcc $(CFLAGS) $(SRC) -o $(DST) && ./$(DST)

clean:
	rm -rf *.out core core.* *.o *.pyc *.log
