/**
 * Different tool convert audio file to wav/pcm format with different header
 * We need trim wav file's head, then indentify them
 *
 * This tool * trim wav file's head, and generate data part to individual file
 *
 *		./wavData [wav]
 *
 * ref:
 * - RIFF: resource interchange file format
 * https://en.wikipedia.org/wiki/Resource_Interchange_File_Format
 * https://zh.wikipedia.org/zh-cn/%E8%B3%87%E6%BA%90%E4%BA%A4%E6%8F%9B%E6%AA%94%E6%A1%88%E6%A0%BC%E5%BC%8F
 */

#include "stdio.h"

#define		BYTES			4

static inline void usage( char *prog ) {
	printf( "%s [wav]\n"
			"trim wav's head, and get internal data\n",
			prog );
}

static inline char show_char( char c ) {
	return isprint( c ) ? c : '.';
}

static inline int is_data( char *p, int i ) {
	return p[i % BYTES] == 'd' &&
		   p[(i + 1) % BYTES] == 'a' &&
		   p[(i + 2) % BYTES] == 't' &&
		   p[(i + 3) % BYTES] == 'a';
}

static inline int trans( FILE *in, FILE *out ) {
#define		LINE_LEN		4096
	char buf[LINE_LEN] = {0};
	size_t n;
	while (1) {
		n = fread( buf, 1, LINE_LEN, in );
		fwrite( buf, 1, n, out );
		if (n < LINE_LEN) {
			return 0;
		}
	}
}

int main( int argc, char **argv ) {
	if (argc <= 1) {
		usage( argv[0] );
		return 0;
	}

	char *fn_in = argv[1];
	FILE *fp_in = fopen( fn_in, "rb" );
	if (fp_in == NULL) {
		fprintf( stderr, "cannot open file=%s\n", fn_in );
		goto error;
	}

	// search 'data' header
	char fourcc[BYTES] = {0};
	int c;
	size_t i, j;
	for (i = 0; !is_data( fourcc, i ); i++ ) {
		c = fgetc( fp_in );
		if (c == EOF) {
			fprintf( stderr, "file=%s i=%lu: file is too short\n", fn_in, i );
			goto error;
		}
		fourcc[i % BYTES] = c;
	}
	// eat 4 bytes, which store data's length
	for (j = 0; j != BYTES; i++, j++) {
		fgetc( fp_in );
	}

#define		FILENAME_LEN	4096
	char fn_out[FILENAME_LEN] = {0};
	int len = snprintf( fn_out, FILENAME_LEN, "%s.raw", fn_in );
	if (len == FILENAME_LEN) {
		fprintf( stderr, "output filename=%s is too long\n", fn_out );
		goto error;
	}
	FILE *fp_out = fopen( fn_out, "wb" );
	trans( fp_in, fp_out );

	fclose( fp_in );
	fclose( fp_out );
	return 0;
error:
	fclose( fp_in );
	return -1;
}
