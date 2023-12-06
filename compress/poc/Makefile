
test: a.out
	./a.out in 1
a.out: bitcnt.c
	gcc bitcnt.c
xxx:
	#./a.out enwik6 1 | cut -d ' ' -f2 | ../tool/entropy
	#./a.out enwik6 2 | cut -d ' ' -f2 | ../tool/entropy
	#./a.out enwik6 8 | cut -d ' ' -f2 | ../tool/entropy
	./a.out enwik6 16 | cut -d ' ' -f2 | ../tool/entropy
