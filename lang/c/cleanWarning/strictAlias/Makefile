# with gcc 4.8 version, have to point strict-aliasing level = 1
a.out:testStrictAlias.c
	gcc -Wall -fstrict-aliasing -Wstrict-aliasing=1 testStrictAlias.c
run:
	./a.out
clean:
	rm -rf a.out
