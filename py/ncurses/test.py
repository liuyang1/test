#! /bin/python
import curses

scr=curses.initscr()

scr.border(0)
scr.addstr(12,25,"Python curses in action!")
scr.refresh()

scr.getch()

curses.endwin()
