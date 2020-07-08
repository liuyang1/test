#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

int main() {
    initscr();
    noecho();
    raw();
    cbreak();
    curs_set(FALSE);

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x); // stdscr from initscr, get max_y, max_x

    clear();
    mvprintw(0, 0, "win=%d,%d", max_x, max_y);;
    refresh();

    sleep(1);

    mousemask(ALL_MOUSE_EVENTS, NULL);
    keypad(stdscr, TRUE);

    mvprintw(3, 3, "test mouse");
    MEVENT ev;
    while (true) {
        // clear();
        int ch = getch();
        switch (ch) {
            case KEY_MOUSE:
                if (getmouse(&ev) == OK) {
                    if (ev.bstate & BUTTON1_CLICKED) { // works
                        mvprintw(1, 1, "mouse %d,%d button1 st=%08lx",
                                 ev.x, ev.y, ev.bstate);
                    } else if (ev.bstate & BUTTON2_CLICKED) { // !!! It doesn't work
                        mvprintw(2, 1, "mouse %d,%d button2 st=%08lx",
                                 ev.x, ev.y, ev.bstate);
                    } else if (ev.bstate & BUTTON3_CLICKED) { // works
                        mvprintw(3, 1, "mouse %d,%d button3 st=%08lx",
                                 ev.x, ev.y, ev.bstate);
                    } else {
                        mvprintw(4, 1, "mouse %d,%d un",
                                 ev.x, ev.y, ev.bstate);
                    }
                }
                break;
            case 'q':
                goto end;
                break;
            default:
                mvprintw(5, 1, "unknown ch=%02x/%c", ch, isprint(ch) ? ch : ' ');
                break;
        }
        refresh();
        usleep(30 * 1000);
    }

end:

    endwin();

    return 0;
}
