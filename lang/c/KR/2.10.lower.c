#include <stdio.h>
#include <ctype.h>

int lower(int c) {
	return c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c;
}

#define CASE(c) {int r = lower(c), ex = tolower(c); \
		printf("%c -> %c ?= %c %s\n", c, r, ex, r == ex ? "PASS" : "fail"); }

#if 0
int main() {
	CASE('a');
	CASE('z');
	CASE('A');
	CASE('Z');
	CASE('1');
	return 0;
}
#endif