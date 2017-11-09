DST=factor

target: $(DST)

factor: factor.c
	gcc -Wall -Werror -g $^ -o $@

run: factor
	./factor 110 120 97 65536 512 256 768 384 12416 24832
	./factor 0 1

perf: factor
	./factor 1299721 2750161 4256249 5800139 8960467 15485867
	./factor 2147483629

check: factor
	valgrind ./factor 110 120 97 65536 512 256 768 384 12416 24832
	valgrind ./factor 0 1
	valgrind ./factor 1299721

clean:
	rm -rf $(DST)
