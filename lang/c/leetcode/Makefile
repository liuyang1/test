SRC=backStrCmp.clean.844.c
# DST=$(patsubst %.c,%.out,$(SRC))
DST=a.out
CFLAGS=-Wall -g -Og -fsanitize=address -lasan
$(info src: $(SRC) -> dst: $(DST))

$(DST):$(SRC)
	gcc $(CFLAGS) $(SRC) -o $(DST)

run: $(DST)
	./$(DST)

clean:
	rm -rf *.out core core.* *.o *.pyc *.log

bt:
	gdb $(DST) core

grind: $(DST)
	valgrind ./$(DST)

EXT=$(HOME)
drop:
	cp $(SRC) $(EXT)

recall:
	rm $(EXT)/$(SRC)
