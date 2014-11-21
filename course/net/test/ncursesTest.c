#include <unistd.h>
#include <stdlib.h>
#include <curses.h>

int main(){
	initscr();

	move(5,15);
	printw("%s","hello,world!\n");
	refresh();
	
	while(getchar()!='q')
		;

	endwin();
	exit(EXIT_SUCCESS);
}
