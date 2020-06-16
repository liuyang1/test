no=03
src=$(wildcard $(no)*.c)

run: a.out
	./a.out < $(no).in
test: a.out
	./a.out < $(no).in | diff $(no).out -
a.out: $(src)
	gcc $(src)
clean:
	rm -rf a.out core
