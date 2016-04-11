#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

bool isprint(char c) {
	return c >= ' ' && c <= '~';
}
bool any_c(char c, const char s[]) {
	for (; *s != '\0'; s++) {
		if (c == *s) {
			return true;
		}
	}
	return false;
}
bool isescape2(char c) {
	return any_c(c, "\a\b\f\r\n\t\v\\\'\"\?");
}
char hex(int c) {
	if (c >= 0 && c <= 9) {
		return c + '0';
	} else if (c >= 10 && c <= 15) {
		return c + 'a' - 10;
	} else {
		return c;
	}
}
int hexr(char c) {
	if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	} else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	} else if (c >= '0' && c <= '9') {
		return c - '0';
	} else {
		return c;
	}
}

// transfer visible newline, tab to \n, \t
int escape(char *dest, int lim, char *src) {
	int i, j;
	char c;
	for (i = 0, j = 0; src[i] != '\0' && j < lim - 1; i++) {
		c = src[i];
		if (isescape2(c)) {
			if (j + 2 >= lim) {
				break;
			}
			dest[j++] = '\\';
			switch (c) {
			case '\a': dest[j++] = 'a'; break;
			case '\b': dest[j++] = 'b'; break;
			case '\f': dest[j++] = 'f'; break;
			case '\n': dest[j++] = 'n'; break;
			case '\r': dest[j++] = 'r'; break;
			case '\t': dest[j++] = 't'; break;
			case '\v': dest[j++] = 'v'; break;
			case '\\': dest[j++] = '\\'; break;
			case '\'': dest[j++] = '\''; break;
			case '\"': dest[j++] = '"'; break;
			case '\?': dest[j++] = '?'; break;
			default:
				break;
			}
		} else if (isprint(c)) {
			if (j + 1 >= lim) {
				break;
			}
			dest[j++] = c;
		} else {
			if (j + 4 >= lim) {
				break;
			}
			dest[j++] = '\\';
			dest[j++] = 'x';
			dest[j++] = hex(c / 16);
			dest[j++] = hex(c % 16);
		}
	}
	dest[j] = '\0';
	return j;
}
int unescape(char *dest, int lim, char *src) {
	int i, j;
	char c;
	for (i = j = 0; src[i] != '\0' && j + 1 < lim; i++) {
		c = src[i];
		if (c == '\\') {
			i++;
			switch (src[i]) {
			case 'x': dest[j++] = hexr(src[i + 1]) * 16 + hexr(src[i + 2]); i += 2; break;
			case 'a': dest[j++] = '\a'; break;
			case 'b': dest[j++] = '\b'; break;
			case 'f': dest[j++] = '\f'; break;
			case 'n': dest[j++] = '\n'; break;
			case 'r': dest[j++] = '\r'; break;
			case 'v': dest[j++] = '\v'; break;
			case 't': dest[j++] = '\t'; break;
			case '\'': dest[j++] = '\''; break;
			case '\"': dest[j++] = '\"'; break;
			case '\\': dest[j++] = '\\'; break;
			case '\?': dest[j++] = '\?'; break;
			default: break;
			}
		} else {
			dest[j++] = c;
		}
	}
	dest[j] = '\0';
	return j;
}

#define STRLEN 32
#define CASE0(s) {char r[STRLEN]; int len = escape(r, STRLEN, s); \
printf("%s(%d) -> %s(%d)\n", s, strlen(s), r, len); }
#define CASE1(s) {char r[STRLEN]; int len = unescape(r, STRLEN, s); \
printf("%s(%d) -> %s(%d)\n", s, strlen(s), r, len); }
#define CASE2(s) {char t[STRLEN], r[STRLEN]; \
int len = escape(t, STRLEN, s); \
len = unescape(r, STRLEN, t); \
printf("%s -> %s -> %s %s\n", s, t, r, strcmp(s, r) == 0 ? "PASS" : "fail"); }
#define CASE3(s) {char t[STRLEN], r[STRLEN]; \
int len = unescape(t, STRLEN, s); \
len = escape(r, STRLEN, t); \
printf("%s -> %s -> %s %s\n", s, t, r, strcmp(s, r) == 0 ? "PASS" : "fail"); }

int test() {
	CASE0("abc");
	CASE0("a\tc");
	CASE0("a\bc");
	CASE0("a\b\nc");
	CASE0("a\'\"\\c");
	CASE0("ac\x0e");
	CASE0("ac\x7f");
	CASE1("abc");
	CASE1("a\\tc");
	CASE1("a\\bc");
	CASE1("a\\b\\nc");
	CASE1("a\\\'\\\"\\\\c");
	CASE1("ac\\x0e");
	CASE1("ac\\x7f");
	CASE2("abc");
	CASE2("a\tc");
	CASE2("a\bc");
	CASE2("a\b\nc");
	CASE2("a\'\"\\c");
	CASE2("ac\x0e");
	CASE2("ac\x7f");
	CASE3("abc");
	CASE3("a\\tc");
	CASE3("a\\bc");
	CASE3("a\\b\\nc");
	CASE3("a\\\'\\\"\\\\c");
	CASE3("ac\\x0e");
	CASE3("ac\\x7f");
	return 0;
}
#if 0
int main() {
	return test();
}
#endif