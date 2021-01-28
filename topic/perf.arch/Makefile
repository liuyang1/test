CFLAGS=-Wall -Werror -g -O3
CFLAGS+=-mmmx -msse -msse2 -mfpmath=sse -mavx

run: a.out
	./a.out

# -mavx
sample: sample.c
	gcc $(CFLAGS) $^ -o $@
	./$@

a.out: matMul.c
	gcc $(CFLAGS) $^ -o $@

clean:
	rm -rf a.out sample core*
