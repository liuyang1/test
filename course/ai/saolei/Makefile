#a.out:main.cpp
#	g++ main.cpp
TARGET=mine

all: $(TARGET)
mouse: mouse.c
	gcc mouse.c -lncurses -o $@
breakout: breakout.c
	gcc breakout.c -lncurses -o $@

run: $(all)
	./$(TARGET)

mine:mine.c
	gcc -g -Wall mine.c -o mine -lncurses && ./mine
clean:
	rm -rf a.out core
