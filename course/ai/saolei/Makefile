#a.out:main.cpp
#	g++ main.cpp
a.out: mouse.c
	gcc mouse.c -lncurses
tmine: tmine.c
	gcc tmine.c -lncurses -o tmine

run: a.out
	./a.out

mine:mine.c
	gcc -g -Wall mine.c -o mine && ./mine
clean:
	rm -rf a.out core
