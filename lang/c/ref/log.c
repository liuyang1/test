#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

struct st_t {
	int a;
	int b;
};

enum _enum_st {
	idle = 0,
	running = 1,
};

static const char *show_enum( enum _enum_st e ) {
	switch (e) {
		case idle: return "idle";
		case running: return "running";
		default: return "unknow enum";
	}
}

static const char *show_bool( bool b ) {
	return b ? "true" : "false";
}

static char show_char( char c ) {
	return isprint( c ) ? c : '.';
}

#define FOURCC_BUFLEN      5
static char *show_fourcc( uint32_t n, char *buf, size_t buflen ) {
	assert( buflen >= FOURCC_BUFLEN );
	snprintf( buf, buflen, "0x%08x/'%c%c%c%c'",
			  n,
			  show_char((n & 0xff000000) >> 24 ),
			  show_char((n & 0x00ff0000) >> 16 ),
			  show_char((n & 0x0000ff00) >> 8 ),
			  show_char((n & 0x000000ff)));
	return buf;
}

int main() {
	char c = 42;
	printf( "char: 0x%02x/'%c'\n", c, show_char( c ));

	int a = 42;
	printf( "hex: 0x%08x\n", a );
	printf( "dec: %d\n", a );

	printf( "boolean: %s %s\n", show_bool( 0 ), show_bool( 1 ));

	char *p = (char *)0xffff0000;
	printf( "pointer: %p %p\n", NULL, p );

	struct st_t st = {.a = 42, .b = 4};
	printf( "struct: st.a=%d st.b=%d\n", st.a, st.b );

	enum _enum_st e = running;
	printf( "enum: e=%d/%s\n", e, show_enum( e ));

	printf( "errno: err=%d/%s\n", errno, strerror( errno ));

	uint32_t fmt = 0x34424752;
	char buf[FOURCC_BUFLEN] = {0};
	printf( "fourcc: %s\n", show_fourcc( fmt, buf, FOURCC_BUFLEN ));

	fmt = 0x0;
	printf( "fourcc: %s\n", show_fourcc( fmt, buf, FOURCC_BUFLEN ));

	return 0;
}
