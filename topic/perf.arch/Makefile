CFLAGS=-O3
CFLAGS+=-mmmx -msse -msse2 -mfpmath=sse -mavx -march=native

run: a.out
	./a.out

# -mavx
sample: sample.c
	gcc $(CFLAGS) $^ -o $@
	./$@

a.out: matMul.c
	gcc $(CFLAGS) $^ -o $@
	objdump -D a.out > a.asm

clean:
	rm -rf a.out sample core*
