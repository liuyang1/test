CFLAGS=-Wall -g
LDFLAGS=-lncursesw -lm
sst: sst.000.c
	gcc $(CFLAGS) $^ $(LDFLAGS) -o $@

run: sst
	./sst

clean:
	rm -rf sst core
