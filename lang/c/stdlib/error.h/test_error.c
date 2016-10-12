#include <stdio.h>
#include <error.h> // include functions related to error
#include <errno.h>

int status = 1, errnum = 2;

void try_error() {
    printf("flush stdout, then write to stderr\n"
           "format: [program name]: [error message]: `strerror(errnum)`\n"
           "then exit program with `status`"
           );
    error(status, errnum, "error message");
}

void try_error_at_line() {
    printf("like error function, but add filename, and linenum\n");
    error_at_line(status, errnum, __FILE__, __LINE__, "error message");
}

void print_progname(void) {
    fprintf(stderr, "prog name ::");
}

void try_error_print_progname() {
    error_print_progname = print_progname;
    error(status, errnum, "error message");
}

int main() {
    // error function will call exit(), so have to test one case per one time
    try_error();
    try_error_at_line();
    try_error_print_progname();
    return 0;
}
