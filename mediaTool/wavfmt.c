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

static inline uint32_t fourcc(char *p)
{
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

typedef struct {
    uint32_t d;
    uint16_t w[2];
    uint8_t b[8];
} GUID;

// x-0-10-80-aa00389b71
// 1: pcm, 2: adpcm, 3: float, 0x92: ac3
static GUID ksmedia = {
    .d = 0xFFFFFFFF,
    .w = {0x00, 0x10},
    .b = {0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71},
};

static inline isKSMedia(GUID * id) {
    return id->w[0] == ksmedia.w[0] && id->w[1] == ksmedia.w[1] &&
           id->b[0] == ksmedia.b[0] && id->b[1] == ksmedia.b[1] &&
           id->b[2] == ksmedia.b[2] && id->b[3] == ksmedia.b[3] &&
           id->b[4] == ksmedia.b[4] && id->b[5] == ksmedia.b[5] &&
           id->b[6] == ksmedia.b[6] && id->b[7] == ksmedia.b[7];
}

static inline char *show_guid(GUID *id, char *s, size_t n)
{
    size_t c = snprintf(s, n, "%08X-%04X-%04X-%02X%02X %02X%02X %02X%02X %02X%02X",
                        id->d,
                        id->w[0], id->w[1],
                        id->b[0], id->b[1], id->b[2], id->b[3],
                        id->b[4], id->b[5], id->b[6], id->b[7]);
    return s;
}

typedef struct {
    uint16_t cb_size;
    union {
        uint16_t valid_bits_per_sample;
        uint16_t samples_per_block; // when bits_per_sample = 0
    } samples;
    uint32_t channel_mask;
    GUID sub_format;
} ExtensibleChk;

enum ChannelGroup {
    FRONT_LEFT = 0x1,
    FRONT_RIGHT = 0x2,
    FRONT_CENTER = 0x4,
    LOW_FREQUENCY = 0x8,
    BACK_LEFT = 0x10,
    BACK_RIGHT = 0x20,
    FRONT_LEFT_CENTER = 0x40,
    FRONT_RIGHT_CENTER = 0x80,
    BACK_CENTER = 0x100,
    SIDE_LEFT = 0x200,
    SIDE_RIGHT = 0x400,
    TOP_CENTER = 0x800,
    TOP_FRONT_LEFT = 0x1000,
    TOP_FRONT_CENTER = 0x2000,
    TOP_FRONT_RIGHT = 0x4000,
    TOP_BACK_LEFT = 0x8000,
    TOP_BACK_CENTER = 0x10000,
    TOP_BACK_RIGHT = 0x20000,
};

typedef struct {
    uint32_t mask;
    char *name;
} GroupMap;

GroupMap groups[] = {
    {0, "directout"},
    {FRONT_CENTER, "mono"},
    {FRONT_LEFT | FRONT_RIGHT, "stereo"},
    {FRONT_LEFT | FRONT_RIGHT | LOW_FREQUENCY, "2.1"},
    {FRONT_LEFT | FRONT_RIGHT | BACK_LEFT | BACK_RIGHT, "quad"},
    {FRONT_LEFT | FRONT_RIGHT | FRONT_CENTER | BACK_CENTER, "surround"},
    {FRONT_LEFT | FRONT_RIGHT | LOW_FREQUENCY | BACK_LEFT | BACK_RIGHT, "4.1"},
    {FRONT_LEFT | FRONT_RIGHT | BACK_LEFT | BACK_RIGHT |
     FRONT_CENTER | LOW_FREQUENCY,
     "5.1"},
    {FRONT_LEFT | FRONT_RIGHT | SIDE_LEFT | SIDE_RIGHT |
     FRONT_CENTER | LOW_FREQUENCY,
     "5.1 surround"},
    {FRONT_LEFT | FRONT_RIGHT | BACK_LEFT | BACK_RIGHT |
     FRONT_CENTER | LOW_FREQUENCY | FRONT_LEFT_CENTER | FRONT_RIGHT_CENTER,
     "7.1"},
    {FRONT_LEFT | FRONT_RIGHT | BACK_LEFT | BACK_RIGHT |
     FRONT_CENTER | LOW_FREQUENCY | SIDE_LEFT | SIDE_RIGHT,
     "7.1 surround"},
};

char *mask_map_verbose[] = {
    "Front Left",
    "Front Right",
    "Front Center",
    "Low Frequency",
    "Back Left",
    "Back Right",
    "Front Left of Center",
    "Front Right of Center",
    "Back Center",
    "Side Left",
    "Side Right",
    "Top Center",
    "Top Front Left",
    "Top Front Center",
    "Top Front Right",
    "Top Back Left",
    "Top Back Center",
    "Top Back Right",
};
char *mask_map_short[] = {
    "L", "R", "C", "LFE",
    "Lb", "Rb", "Lfc", "Rfc",
    "Cb", "Ls", "Rs", "Ct",
    "Ltf", "Ctf", "Rtf", "Ltb", "Ltc", "Ctc", "Rtc",
};

static inline char *show_channel_mask_map(uint32_t m, char *s, size_t n,
                                          char *map[], size_t map_size)
{
    size_t i, c;
    for (i = c = 0; i < 32 && i < map_size; i++) {
        if (m & (1 << i)) {
            c += snprintf(s + c, n - c - 1, "%s,", map[i]);
        }
    }
    return s;
}

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
static inline char *show_channel_mask_group(uint32_t m)
{
    size_t i;
    for (i = 0; i != ARRAY_SIZE(groups); i++) {
        if (m == groups[i].mask) {
            return groups[i].name;
        }
    }
    return "unknown group";
}

typedef struct {
    uint32_t sample_num;
} FactChk;

static inline void usage(char *prog)
{
    printf("%s [wav]\n"
           "trim wav's head, and get internal data\n",
           prog);
}

static inline char show_char(char c)
{
    return isprint(c) ? c : '.';
}

#define FOURCC_BUFLEN      18   // align with show_fourcc format
static inline char *show_fourcc(uint32_t n, char *buf)
{
    // snprintf( buf, FOURCC_BUFLEN, "0x%08x/'%c%c%c%c'",
    //        n,
    snprintf(buf, FOURCC_BUFLEN, "'%c%c%c%c'",
             show_char((n & 0x000000ff)),
             show_char((n & 0x0000ff00) >> 8),
             show_char((n & 0x00ff0000) >> 16),
             show_char((n & 0xff000000) >> 24));
    return buf;
}

#define WAVE_FORMAT_EXTENSIBLE 0xfffe
static inline const char *show_audio_format(uint16_t audio_format)
{
    switch (audio_format) {
    case 1: return "PCM linear";
    case 2: return "MS ADPCM";
    case 3: return "IEEEE float";
    case 6: return "A-law";
    case 7: return "Mu-law";
    case 0x11: return "DVI ADPCM";
    case 0x55: return "MPEG Layer3";
    case 0x92: return "Dolby AC3 Spdif";
    case 0x161: return "WM Audio2";
    case 0x162: return "WM Audio3";
    case 0x164: return "WM AUdio Spdif";
    case WAVE_FORMAT_EXTENSIBLE: return "wave format extensible";
    default: return "unknown audio format";
    }
}

static inline int die(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(-1);
}

static void trans(uint8_t *src, uint32_t size, char *fn_out)
{
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

static void process_chunk(const void *p_in)
{
    Chunk *p = (Chunk *)p_in;
    char buf[FOURCC_BUFLEN];
    printf("id=%s size=%u\n", show_fourcc(p->id, buf), p->size);

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
        printf(L "audio_format=%hu/'%s' channel_num=%hu "
               "sample_rate=%u byte_rate=%u "
               "block_align=%hu bits_per_sample=%hu\n",
               chk->audio_format, show_audio_format(chk->audio_format),
               chk->channel_num, chk->sample_rate, chk->byte_rate,
               chk->block_align, chk->bits_per_sample);
        ExtensibleChk *ext = (ExtensibleChk *)(chk + 1);
        printf(L "cb_size=%d\n", ext->cb_size);
        if (chk->audio_format == WAVE_FORMAT_EXTENSIBLE || ext->cb_size != 0) {
            // cb_size should be 22
            if (chk->bits_per_sample == 0) {
                printf(L "samples_per_block=%hu\n",
                       ext->samples.samples_per_block);
            } else {
                printf(L "valid_bits_per_sample=%hu\n",
                       ext->samples.valid_bits_per_sample);
            }
            char s[256];
            printf(L "channel_mask=%#x\n", ext->channel_mask);
            printf(L L "channel=[%s]\n",
                   show_channel_mask_map(ext->channel_mask, s, 256,
                                         mask_map_short, ARRAY_SIZE(mask_map_short)));
            printf(L L "channel=[%s]\n",
                   show_channel_mask_map(ext->channel_mask, s, 256,
                                         mask_map_verbose, ARRAY_SIZE(mask_map_verbose)));
            printf(L L "channel_group='%s'\n",
                   show_channel_mask_group(ext->channel_mask));
            printf(L "sub_format=%d/'%s' GUID=%s%s\n",
                   ext->sub_format.d, show_audio_format(ext->sub_format.d),
                   show_guid(&ext->sub_format, s, 256),
                   isKSMedia(&ext->sub_format) ? "/ksmedia" : "");
        }

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
    } else if (p->id == fourcc("fact")) {
        FactChk *fact = (FactChk *)p->payload;
        printf(L "fact chunk, number of samples (per channel) = %u\n",
               fact->sample_num);
        process_chunk(p->payload + p->size);
    } else {
        printf(L "unknown chunk id\n");
    }
}

int main(int argc, char **argv)
{
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
