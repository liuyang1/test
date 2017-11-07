TOP=$(shell git rev-parse --show-toplevel)

SRC=string.c
DST=a.out
INC=-I $(TOP)/lang/c/KR

a.out: $(SRC)
	@gcc -Wall -Werror $(INC) $^ $>
	@echo CC $^

run: $(DST)
	./$(DST)

check: $(DST)
	cppcheck $(SRC)
	valgrind ./$(DST)

clean:
	rm -rf $(DST)
