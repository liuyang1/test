fb.1.5: fb.1.5.l fb.1.5.y
	bison -d fb.1.5.y
	flex fb.1.5.l
	gcc -o $@ fb.1.5.tab.c lex.yy.c -lfl

clean:
	rm -rf lex.yy.c *.tab.c *.tab.h fb.1.5

run:
	./fb.1.5
