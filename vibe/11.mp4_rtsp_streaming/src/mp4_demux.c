#define _POSIX_C_SOURCE 200112L
#include "mp4_demux.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

#define FOURCC(a,b,c,d) ((uint32_t)(a)<<24|(b)<<16|(c)<<8|(d))

/* ---- internal: stbl parsing temporaries ---- */
typedef struct {
    uint32_t  entry_count;
    uint32_t *sizes;           /* stsz */
    uint32_t  stsc_count;
    uint32_t (*stsc)[3];       /* first_chunk, samples_per_chunk, sdi */
    uint32_t  stco_count;
    uint64_t *chunk_offsets;   /* stco / co64 */
    uint32_t  stts_count;
    uint32_t (*stts)[2];       /* count, delta */
    uint32_t  ctts_count;
    int32_t  (*ctts)[2];       /* count, offset */
    uint32_t  stss_count;
    uint32_t *stss;            /* sync sample numbers (1-based) */
} stbl_t;

static int read_box_header(FILE *fp, uint32_t *type, uint64_t *size, uint64_t *data_off) {
    uint8_t h[8];
    uint64_t pos = ftello(fp);
    if (fread(h, 1, 8, fp) != 8) return -1;
    *size = rd32(h);
    *type = rd32(h + 4);
    *data_off = pos + 8;
    if (*size == 1) {
        uint8_t h2[8];
        if (fread(h2, 1, 8, fp) != 8) return -1;
        *size = rd64(h2);
        *data_off = pos + 16;
    } else if (*size == 0) {
        fseeko(fp, 0, SEEK_END);
        *size = ftello(fp) - pos;
        fseeko(fp, *data_off, SEEK_SET);
    }
    return 0;
}

static int read_full_box(FILE *fp, uint8_t *version, uint32_t *flags) {
    uint8_t h[4];
    if (fread(h, 1, 4, fp) != 4) return -1;
    *version = h[0];
    *flags = (h[1] << 16) | (h[2] << 8) | h[3];
    return 0;
}

/* read N bytes into malloc'd buffer */
static uint8_t *read_alloc(FILE *fp, uint32_t n) {
    uint8_t *buf = malloc(n);
    if (!buf) return NULL;
    if (fread(buf, 1, n, fp) != n) { free(buf); return NULL; }
    return buf;
}

/* ---- parse avcC ---- */
static void parse_avcc(mp4_track_t *t, const uint8_t *d, uint32_t len) {
    if (len < 8) return;
    t->nalu_len_size = (d[4] & 3) + 1;
    int nsps = d[5] & 0x1f;
    const uint8_t *p = d + 6;
    if (nsps > 0 && p + 2 <= d + len) {
        t->sps_len = rd16(p); p += 2;
        if (p + t->sps_len <= d + len) {
            t->sps = malloc(t->sps_len);
            memcpy(t->sps, p, t->sps_len);
            p += t->sps_len;
        }
    }
    if (p + 1 <= d + len) {
        int npps = *p++;
        if (npps > 0 && p + 2 <= d + len) {
            t->pps_len = rd16(p); p += 2;
            if (p + t->pps_len <= d + len) {
                t->pps = malloc(t->pps_len);
                memcpy(t->pps, p, t->pps_len);
            }
        }
    }
}

/* ---- parse esds to extract AudioSpecificConfig ---- */
static int esds_read_tag(const uint8_t **pp, const uint8_t *end,
                         uint8_t *tag, uint32_t *sz) {
    if (*pp >= end) return -1;
    *tag = *(*pp)++;
    *sz = 0;
    uint8_t b;
    do {
        if (*pp >= end) return -1;
        b = *(*pp)++;
        *sz = (*sz << 7) | (b & 0x7f);
    } while (b & 0x80);
    return 0;
}

static void parse_esds(mp4_track_t *t, const uint8_t *d, uint32_t len) {
    const uint8_t *p = d, *end = d + len;
    uint8_t tag; uint32_t sz;

    /* ES_Descriptor (tag 3) */
    if (esds_read_tag(&p, end, &tag, &sz) < 0 || tag != 3) return;
    if (p + 3 > end) return;
    p += 3; /* skip ES_ID(2) + flags(1) */

    /* DecoderConfigDescriptor (tag 4) */
    if (esds_read_tag(&p, end, &tag, &sz) < 0 || tag != 4) return;
    if (p + 13 > end) return;
    p += 13; /* skip objectTypeIndication + fixed fields */

    /* DecoderSpecificInfo (tag 5) */
    if (esds_read_tag(&p, end, &tag, &sz) < 0 || tag != 5) return;
    if (sz < 2 || p + sz > end) return;

    t->asc_len = sz;
    t->asc = malloc(sz);
    memcpy(t->asc, p, sz);
    uint8_t byte0 = p[0], byte1 = p[1];
    t->profile = (byte0 >> 3) - 1;
    t->freq_index = ((byte0 & 7) << 1) | (byte1 >> 7);
    t->chan_config = (byte1 >> 3) & 0xf;
}

/* ---- parse sample table boxes ---- */
static void parse_stsz(FILE *fp, stbl_t *s, uint64_t box_end) {
    (void)box_end;
    uint8_t ver; uint32_t fl;
    read_full_box(fp, &ver, &fl);
    uint8_t h[8]; fread(h, 1, 8, fp);
    uint32_t sample_size = rd32(h);
    s->entry_count = rd32(h + 4);
    s->sizes = malloc(s->entry_count * 4);
    if (sample_size != 0) {
        for (uint32_t i = 0; i < s->entry_count; i++) s->sizes[i] = sample_size;
    } else {
        for (uint32_t i = 0; i < s->entry_count; i++) {
            uint8_t b[4]; fread(b, 1, 4, fp); s->sizes[i] = rd32(b);
        }
    }
}

static void parse_stsc(FILE *fp, stbl_t *s) {
    uint8_t ver; uint32_t fl;
    read_full_box(fp, &ver, &fl);
    uint8_t h[4]; fread(h, 1, 4, fp);
    s->stsc_count = rd32(h);
    s->stsc = malloc(s->stsc_count * sizeof(uint32_t[3]));
    for (uint32_t i = 0; i < s->stsc_count; i++) {
        uint8_t e[12]; fread(e, 1, 12, fp);
        s->stsc[i][0] = rd32(e);
        s->stsc[i][1] = rd32(e + 4);
        s->stsc[i][2] = rd32(e + 8);
    }
}

static void parse_stco(FILE *fp, stbl_t *s, int is64) {
    uint8_t ver; uint32_t fl;
    read_full_box(fp, &ver, &fl);
    uint8_t h[4]; fread(h, 1, 4, fp);
    s->stco_count = rd32(h);
    s->chunk_offsets = malloc(s->stco_count * 8);
    for (uint32_t i = 0; i < s->stco_count; i++) {
        if (is64) {
            uint8_t b[8]; fread(b, 1, 8, fp);
            s->chunk_offsets[i] = rd64(b);
        } else {
            uint8_t b[4]; fread(b, 1, 4, fp);
            s->chunk_offsets[i] = rd32(b);
        }
    }
}

static void parse_stts(FILE *fp, stbl_t *s) {
    uint8_t ver; uint32_t fl;
    read_full_box(fp, &ver, &fl);
    uint8_t h[4]; fread(h, 1, 4, fp);
    s->stts_count = rd32(h);
    s->stts = malloc(s->stts_count * sizeof(uint32_t[2]));
    for (uint32_t i = 0; i < s->stts_count; i++) {
        uint8_t e[8]; fread(e, 1, 8, fp);
        s->stts[i][0] = rd32(e);
        s->stts[i][1] = rd32(e + 4);
    }
}

static void parse_ctts(FILE *fp, stbl_t *s) {
    uint8_t ver; uint32_t fl;
    read_full_box(fp, &ver, &fl);
    uint8_t h[4]; fread(h, 1, 4, fp);
    s->ctts_count = rd32(h);
    s->ctts = malloc(s->ctts_count * sizeof(int32_t[2]));
    for (uint32_t i = 0; i < s->ctts_count; i++) {
        uint8_t e[8]; fread(e, 1, 8, fp);
        s->ctts[i][0] = (int32_t)rd32(e);
        s->ctts[i][1] = (int32_t)rd32(e + 4);
    }
}

static void parse_stss(FILE *fp, stbl_t *s) {
    uint8_t ver; uint32_t fl;
    read_full_box(fp, &ver, &fl);
    uint8_t h[4]; fread(h, 1, 4, fp);
    s->stss_count = rd32(h);
    s->stss = malloc(s->stss_count * 4);
    for (uint32_t i = 0; i < s->stss_count; i++) {
        uint8_t b[4]; fread(b, 1, 4, fp); s->stss[i] = rd32(b);
    }
}

/* build sample table from stbl sub-boxes */
static int build_samples(mp4_track_t *t, stbl_t *s) {
    if (!s->sizes || s->entry_count == 0) return -1;
    t->sample_count = s->entry_count;
    t->samples = calloc(t->sample_count, sizeof(mp4_sample_t));

    /* sizes */
    for (uint32_t i = 0; i < t->sample_count; i++)
        t->samples[i].size = s->sizes[i];

    /* offsets from stsc + stco */
    if (s->chunk_offsets && s->stsc) {
        uint32_t si = 0;
        for (uint32_t ci = 0; ci < s->stco_count && si < t->sample_count; ci++) {
            /* find samples_per_chunk for this chunk */
            uint32_t spc = 0;
            for (uint32_t j = 0; j < s->stsc_count; j++) {
                if (ci + 1 >= s->stsc[j][0]) spc = s->stsc[j][1];
            }
            uint64_t off = s->chunk_offsets[ci];
            for (uint32_t k = 0; k < spc && si < t->sample_count; k++) {
                t->samples[si].offset = off;
                off += t->samples[si].size;
                si++;
            }
        }
    }

    /* DTS from stts */
    if (s->stts) {
        uint64_t dts = 0;
        uint32_t si = 0;
        for (uint32_t i = 0; i < s->stts_count && si < t->sample_count; i++) {
            for (uint32_t j = 0; j < s->stts[i][0] && si < t->sample_count; j++) {
                t->samples[si].dts = dts;
                t->samples[si].pts = dts;
                dts += s->stts[i][1];
                si++;
            }
        }
    }

    /* CTS offset from ctts → PTS = DTS + offset */
    if (s->ctts) {
        uint32_t si = 0;
        for (uint32_t i = 0; i < s->ctts_count && si < t->sample_count; i++) {
            for (int32_t j = 0; j < s->ctts[i][0] && si < t->sample_count; j++) {
                t->samples[si].pts = t->samples[si].dts + s->ctts[i][1];
                si++;
            }
        }
    }

    /* sync samples from stss (if absent, all are sync) */
    if (s->stss) {
        for (uint32_t i = 0; i < s->stss_count; i++) {
            uint32_t idx = s->stss[i] - 1;
            if (idx < t->sample_count) t->samples[idx].is_sync = 1;
        }
    } else {
        for (uint32_t i = 0; i < t->sample_count; i++)
            t->samples[i].is_sync = 1;
    }

    return 0;
}

static void stbl_free(stbl_t *s) {
    free(s->sizes); free(s->stsc); free(s->chunk_offsets);
    free(s->stts); free(s->ctts); free(s->stss);
    memset(s, 0, sizeof(*s));
}

/* ---- recursive box parser ---- */
static int parse_boxes(mp4_demux_t *d, uint64_t start, uint64_t end,
                       int depth, mp4_track_t **cur_track, stbl_t *stbl);

/* container boxes that we recurse into */
static int is_container(uint32_t type) {
    return type == FOURCC('m','o','o','v') || type == FOURCC('t','r','a','k') ||
           type == FOURCC('m','d','i','a') || type == FOURCC('m','i','n','f') ||
           type == FOURCC('s','t','b','l') || type == FOURCC('d','i','n','f') ||
           type == FOURCC('u','d','t','a');
}

static int parse_boxes(mp4_demux_t *d, uint64_t start, uint64_t end,
                       int depth, mp4_track_t **cur_track, stbl_t *stbl) {
    uint64_t pos = start;
    while (pos + 8 <= end) {
        fseeko(d->fp, pos, SEEK_SET);
        uint32_t type; uint64_t size, data_off;
        if (read_box_header(d->fp, &type, &size, &data_off) < 0) break;
        if (size < 8 || pos + size > end) break;
        uint64_t box_end = pos + size;

        if (type == FOURCC('t','r','a','k')) {
            /* new track */
            if (d->track_count >= 8) { pos = box_end; continue; }
            mp4_track_t *t = &d->tracks[d->track_count++];
            memset(t, 0, sizeof(*t));
            stbl_t sb = {0};
            mp4_track_t *ct = t;
            parse_boxes(d, data_off, box_end, depth + 1, &ct, &sb);
            build_samples(t, &sb);
            stbl_free(&sb);
            if (t->codec == FOURCC('a','v','c','1') && d->video_idx < 0)
                d->video_idx = d->track_count - 1;
            else if (t->codec == FOURCC('m','p','4','a') && d->audio_idx < 0)
                d->audio_idx = d->track_count - 1;
        } else if (is_container(type)) {
            parse_boxes(d, data_off, box_end, depth + 1, cur_track, stbl);
        } else if (type == FOURCC('m','d','h','d') && cur_track && *cur_track) {
            uint8_t ver; uint32_t fl;
            read_full_box(d->fp, &ver, &fl);
            if (ver == 0) {
                uint8_t b[12]; fread(b, 1, 12, d->fp);
                (*cur_track)->timescale = rd32(b + 8);
            } else {
                uint8_t b[20]; fread(b, 1, 20, d->fp);
                (*cur_track)->timescale = rd32(b + 16);
            }
        } else if (type == FOURCC('t','k','h','d') && cur_track && *cur_track) {
            uint8_t ver; uint32_t fl;
            read_full_box(d->fp, &ver, &fl);
            if (ver == 0) {
                uint8_t b[12]; fread(b, 1, 12, d->fp);
                (*cur_track)->track_id = rd32(b + 8);
            } else {
                uint8_t b[20]; fread(b, 1, 20, d->fp);
                (*cur_track)->track_id = rd32(b + 16);
            }
        } else if (type == FOURCC('s','t','s','d') && cur_track && *cur_track) {
            /* sample description - parse codec-specific */
            uint8_t ver; uint32_t fl;
            read_full_box(d->fp, &ver, &fl);
            uint8_t h[4]; fread(h, 1, 4, d->fp);
            /* read first entry header */
            uint64_t entry_pos = ftello(d->fp);
            uint32_t etype; uint64_t esize, edata;
            if (read_box_header(d->fp, &etype, &esize, &edata) == 0) {
                (*cur_track)->codec = etype;
                if (etype == FOURCC('a','v','c','1') || etype == FOURCC('a','v','c','3')) {
                    (*cur_track)->codec = FOURCC('a','v','c','1');
                    /* visual sample entry: 78 bytes fixed fields after data_off */
                    fseeko(d->fp, edata + 78, SEEK_SET);
                    /* parse sub-boxes for avcC */
                    uint64_t sub_end = entry_pos + esize;
                    uint64_t sp = ftello(d->fp);
                    while (sp + 8 <= sub_end) {
                        fseeko(d->fp, sp, SEEK_SET);
                        uint32_t st; uint64_t ss, sd;
                        if (read_box_header(d->fp, &st, &ss, &sd) < 0) break;
                        if (st == FOURCC('a','v','c','C')) {
                            uint32_t dlen = (uint32_t)(sp + ss - sd);
                            uint8_t *buf = read_alloc(d->fp, dlen);
                            if (buf) { parse_avcc(*cur_track, buf, dlen); free(buf); }
                        }
                        sp += ss;
                        if (ss < 8) break;
                    }
                } else if (etype == FOURCC('m','p','4','a')) {
                    /* audio sample entry: 28 bytes fixed fields after data_off */
                    /* channels at offset 16, sample_rate at offset 24 (16.16) */
                    fseeko(d->fp, edata + 16, SEEK_SET);
                    uint8_t ainfo[12]; fread(ainfo, 1, 12, d->fp);
                    (*cur_track)->channels = rd16(ainfo);
                    (*cur_track)->sample_rate = rd32(ainfo + 8) >> 16;
                    /* sub-boxes start at edata + 28 */
                    uint64_t sub_end = entry_pos + esize;
                    uint64_t sp = ftello(d->fp);
                    while (sp + 8 <= sub_end) {
                        fseeko(d->fp, sp, SEEK_SET);
                        uint32_t st; uint64_t ss, sd;
                        if (read_box_header(d->fp, &st, &ss, &sd) < 0) break;
                        if (st == FOURCC('e','s','d','s')) {
                            uint8_t ver2; uint32_t fl2;
                            read_full_box(d->fp, &ver2, &fl2);
                            uint32_t dlen = (uint32_t)(sp + ss - ftello(d->fp));
                            uint8_t *buf = read_alloc(d->fp, dlen);
                            if (buf) { parse_esds(*cur_track, buf, dlen); free(buf); }
                        }
                        sp += ss;
                        if (ss < 8) break;
                    }
                }
            }
        } else if (type == FOURCC('s','t','s','z') && stbl) {
            fseeko(d->fp, data_off, SEEK_SET);
            parse_stsz(d->fp, stbl, box_end);
        } else if (type == FOURCC('s','t','s','c') && stbl) {
            fseeko(d->fp, data_off, SEEK_SET);
            parse_stsc(d->fp, stbl);
        } else if (type == FOURCC('s','t','c','o') && stbl) {
            fseeko(d->fp, data_off, SEEK_SET);
            parse_stco(d->fp, stbl, 0);
        } else if (type == FOURCC('c','o','6','4') && stbl) {
            fseeko(d->fp, data_off, SEEK_SET);
            parse_stco(d->fp, stbl, 1);
        } else if (type == FOURCC('s','t','t','s') && stbl) {
            fseeko(d->fp, data_off, SEEK_SET);
            parse_stts(d->fp, stbl);
        } else if (type == FOURCC('c','t','t','s') && stbl) {
            fseeko(d->fp, data_off, SEEK_SET);
            parse_ctts(d->fp, stbl);
        } else if (type == FOURCC('s','t','s','s') && stbl) {
            fseeko(d->fp, data_off, SEEK_SET);
            parse_stss(d->fp, stbl);
        }

        pos = box_end;
    }
    return 0;
}

int mp4_open(mp4_demux_t *d, const char *path) {
    memset(d, 0, sizeof(*d));
    d->video_idx = -1;
    d->audio_idx = -1;
    d->fp = fopen(path, "rb");
    if (!d->fp) { ERR("cannot open %s", path); return -1; }

    fseeko(d->fp, 0, SEEK_END);
    uint64_t file_size = ftello(d->fp);
    fseeko(d->fp, 0, SEEK_SET);

    mp4_track_t *ct = NULL;
    stbl_t sb = {0};
    parse_boxes(d, 0, file_size, 0, &ct, &sb);
    stbl_free(&sb);

    if (d->video_idx < 0 && d->audio_idx < 0) {
        ERR("no video or audio track found");
        mp4_close(d);
        return -1;
    }
    return 0;
}

int mp4_read_sample(mp4_demux_t *d, int track_idx,
                    uint32_t sample_idx, uint8_t *buf, uint32_t buf_size) {
    if (track_idx < 0 || track_idx >= (int)d->track_count) return -1;
    mp4_track_t *t = &d->tracks[track_idx];
    if (sample_idx >= t->sample_count) return -1;
    mp4_sample_t *s = &t->samples[sample_idx];
    if (s->size > buf_size) return -1;
    fseeko(d->fp, s->offset, SEEK_SET);
    if (fread(buf, 1, s->size, d->fp) != s->size) return -1;
    return (int)s->size;
}

void mp4_close(mp4_demux_t *d) {
    for (uint32_t i = 0; i < d->track_count; i++) {
        mp4_track_t *t = &d->tracks[i];
        free(t->samples); free(t->sps); free(t->pps); free(t->asc);
    }
    if (d->fp) fclose(d->fp);
    memset(d, 0, sizeof(*d));
}

void mp4_dump(const mp4_demux_t *d) {
    fprintf(stderr, "tracks: %u  video_idx: %d  audio_idx: %d\n",
            d->track_count, d->video_idx, d->audio_idx);
    for (uint32_t i = 0; i < d->track_count; i++) {
        const mp4_track_t *t = &d->tracks[i];
        char cc[5] = {t->codec>>24, t->codec>>16, t->codec>>8, t->codec, 0};
        fprintf(stderr, "  track[%u]: id=%u codec=%s timescale=%u samples=%u\n",
                i, t->track_id, cc, t->timescale, t->sample_count);
        if (t->sps) fprintf(stderr, "    sps_len=%u pps_len=%u nalu_len_size=%u\n",
                            t->sps_len, t->pps_len, t->nalu_len_size);
        if (t->asc) fprintf(stderr, "    asc_len=%u rate=%u ch=%u profile=%u freq_idx=%u chan_cfg=%u\n",
                            t->asc_len, t->sample_rate, t->channels,
                            t->profile, t->freq_index, t->chan_config);
        if (t->sample_count > 0) {
            fprintf(stderr, "    first sample: off=%lu size=%u dts=%lu pts=%lu sync=%d\n",
                    t->samples[0].offset, t->samples[0].size,
                    t->samples[0].dts, t->samples[0].pts, t->samples[0].is_sync);
            mp4_sample_t *last = &t->samples[t->sample_count - 1];
            fprintf(stderr, "    last  sample: off=%lu size=%u dts=%lu pts=%lu sync=%d\n",
                    last->offset, last->size, last->dts, last->pts, last->is_sync);
        }
    }
}
