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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <ctype.h>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static char *g_fn_in = NULL;

static inline uint32_t fourcc(char *p) {
    return ((uint32_t)(uint8_t)(p[0]) |
            ((uint32_t)(uint8_t)(p[1]) <<  8) |
            ((uint32_t)(uint8_t)(p[2]) << 16) |
            ((uint32_t)(uint8_t)(p[3]) << 24));
}

typedef struct {
    uint32_t id;
    uint32_t size;
    uint8_t payload[0];
} Chunk;

typedef struct {
    uint16_t audio_format;
    uint16_t channel_num;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
} FmtChk;

static inline void usage(char *prog) {
    printf("%s [wav]\n"
           "trim wav's head, and get internal data\n",
           prog);
}

static inline char show_char(char c) {
    return isprint(c) ? c : '.';
}

#define FOURCC_BUFLEN      18   // align with show_fourcc format
static inline char *show_fourcc(uint32_t n, char *buf) {
    // snprintf( buf, FOURCC_BUFLEN, "0x%08x/'%c%c%c%c'",
    //        n,
    snprintf(buf, FOURCC_BUFLEN, "'%c%c%c%c'",
             show_char((n & 0x000000ff)),
             show_char((n & 0x0000ff00) >> 8),
             show_char((n & 0x00ff0000) >> 16),
             show_char((n & 0xff000000) >> 24));
    return buf;
}

static inline const char *show_audio_format(uint16_t audio_format) {
    if (audio_format == 1) {
        return "'PCM linear'";
    } else {
        return "'unknown audio format'";
    }
}

static inline int die(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(-1);
}

static void trans(uint8_t *src, uint32_t size, char *fn_out) {
    int fd = open(fn_out, O_RDWR | O_CREAT, (mode_t)0600);
    if (fd < 0) {
        die("cannot open file=%s\n", fn_out);
    }

    ftruncate(fd, size);    // need enlarge dest file size first
    uint8_t *p = mmap(NULL, size, PROT_WRITE, MAP_SHARED, fd, 0);
    memcpy(p, src, size);
    msync(p, size, MS_SYNC);
    munmap(p, size);

    close(fd);
}

static void process_chunk(void *p_in) {
    Chunk *p = (Chunk *)p_in;
    char buf[FOURCC_BUFLEN];
    printf("id=%s size=%d\n", show_fourcc(p->id, buf), p->size);

#define L      "  "
    if (p->id == fourcc("RIFF")) {
        uint32_t fmt = *(uint32_t *)p->payload;
        printf(L "format=%s\n", show_fourcc(fmt, buf));
        if (fmt != fourcc("WAVE")) {
            printf("unknown format type\n");
            return;
        }

        process_chunk(p->payload + 4);
    } else if (p->id == fourcc("fmt ")) {
        FmtChk *chk = (FmtChk *)p->payload;
        printf(L "audio_format=%hu/%s channel_num=%hu "
               "sample_rate=%u byte_rate=%u "
               "block_align=%hu bits_per_sample=%hu\n",
               chk->audio_format, show_audio_format(chk->audio_format),
               chk->channel_num, chk->sample_rate, chk->byte_rate,
               chk->block_align, chk->bits_per_sample);

        process_chunk(p->payload + p->size);
    } else if (p->id == fourcc("LIST")) {
        uint32_t fmt = *(uint32_t *)p->payload;
        printf(L "format=%s\n", show_fourcc(fmt, buf));
        if (fmt == fourcc("INFO")) {
            process_chunk(p->payload + 4);
        } else if (fmt == fourcc("adtl")) {
            printf(L "unsupported adtl chunk, skip\n");
            process_chunk(p->payload + p->size);
        } else {
            printf(L "unkown format type, skip\n");
            process_chunk(p->payload + p->size);
        }
    } else if (p->id == fourcc("ISFT")) {   // or other info type
        if (p->payload[p->size - 1] != '\0') {
            die("invalid payload in ISFT chunk\n");
        }
        printf(L "text=%s\n", p->payload);
        process_chunk(p->payload + p->size);
    } else if (p->id == fourcc("data")) {
#define     FILENAME_LEN    4096
        char fn_out[FILENAME_LEN] = {0};
        int len = snprintf(fn_out, FILENAME_LEN, "%s.raw", g_fn_in);
        if (len == FILENAME_LEN) {
            die("output filename=%s is too long\n", fn_out);
        }
        printf(L "data chunk, save to %s\n", fn_out);
        trans(p->payload, p->size, fn_out);
    } else {
        printf(L "unknown chunk id\n");
    }
}

int main(int argc, char **argv) {
    if (argc <= 1) {
        usage(argv[0]);
        return 0;
    }

    g_fn_in = argv[1];
    int fd_in = open(g_fn_in, O_RDONLY);
    if (fd_in < 0) {
        die("cannot open file=%s\n", g_fn_in);
    }
    struct stat stat_in;
    fstat(fd_in, &stat_in);
    uint32_t size = stat_in.st_size;

    uint8_t *p = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd_in, 0);
    if (p == MAP_FAILED) {
        die("fail to mmap input file\n");
    }

    Chunk *chk = (Chunk *)p;
    process_chunk(chk);

    munmap(p, size);
    close(fd_in);
    return 0;
}
