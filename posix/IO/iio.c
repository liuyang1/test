/**
 * @file iio.c
 * @brief this file copy from
 * http://rosettacode.org/wiki/Keyboard_input/Flush_the_keyboard_buffer#C
 *
 * setting terminal config to noncanonical mode to get input key immediately
 * @author liuyang<ly@marvell.com>
 * @version 0.0.1
 * @date 2015-01-17
 */
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

/**
 * @brief 
 *
 * want_key 0, setting orignal termios
 * want_key 1: set to noncannonical mode
 * TCSANOW, change immediately
 *
 * @param want_key
 */
void set_mode(int want_key)
{
    static struct termios old, new;
    if (!want_key) {
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
        return;
    }
    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
}

int get_key()
{
    int c = 0;
    fd_set fs;

    FD_ZERO(&fs);
    FD_SET(STDIN_FILENO, &fs);
    select(STDIN_FILENO + 1, &fs, 0, 0, 0);

    if (FD_ISSET(STDIN_FILENO, &fs)) {
        c = getchar();
        set_mode(0);
    }
    return c;
}

int main()
{
    int c = 0;
    while (c != 'q') {
        set_mode(1);
        tcflush(STDIN_FILENO, TCIFLUSH);
        printf("Yes/No/Ignore:>");
        fflush(stdout);
        switch (c = tolower(get_key())) {
            case 'y':
                puts("yes\n");
                putchar('\n');
                break;
            case 'n':
                puts("no\n");
                break;
            case '\t':
                puts("tab\n");
                break;
            default:
                puts("unkown\n");
        }
    }
    return 0;
}
