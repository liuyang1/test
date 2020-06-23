no=05
src=$(wildcard $(no)*.c)
src+=$(wildcard $(no)*.cpp)

run: a.out
	./a.out < $(no).in
test: a.out
	./a.out < $(no).in | diff $(no).out -
chk: ./a.out
	valgrind ./a.out <$(no).in
a.out: $(src)
	gcc -g $(src)
clean:
	rm -rf a.out core

drop:
	cp $(src) $(HOME)
