LD=
# LD=-Wl,-flat_namespace,-undefined,dynamic_lookup

all: single multip

single: weakRef.c
	gcc weakRef.c $(LD) -o single

multip: weakRef.c
	gcc weakRef.c $(LD) -o multip -lpthread

run:
	./single
	./multip

clean:
	rm -rf single multip
