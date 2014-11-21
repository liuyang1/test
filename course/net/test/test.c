#include <curses.h>

#define ESCAPE	27
#define VLINE	'-'
#define HLINE	'|'
#define HEIGHT	5
#define WIDTH	40
int main(){
	initscr();
	WINDOW *recvBd=subwin(stdscr,HEIGHT+2,WIDTH+2,1,1);
	WINDOW *inputBd=subwin(stdscr,HEIGHT+2,WIDTH+2,11,1);
	WINDOW *recvWin=subwin(stdscr,5,40,2,2);
	WINDOW *inputWin=subwin(stdscr,5,40,12,2);
	box(recvBd,HLINE,VLINE);
	box(inputBd,HLINE,VLINE);
	wprintw(recvBd,"recvWin");
	wprintw(inputBd,"inputWin");
	cbreak();
	keypad(stdscr,TRUE);
	start_color();

	refresh();

	scrollok(recvWin,1);
	scrollok(inputWin,1);
	char str[80];
	int i;
	for(i=0;;i++){
		int key=wgetch(inputWin);
		if(key==ESCAPE){
			break;
		}
		wscanw(inputWin,"%s",str);
		touchwin(inputWin);
		wrefresh(inputWin);

		wprintw(recvWin," %c%s\n",key,str);
		touchwin(recvWin);
		wrefresh(recvWin);
	}

	delwin(recvWin);
	delwin(inputWin);
	delwin(recvBd);
	delwin(inputBd);
	endwin();
	return 0;
}
