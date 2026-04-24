/* Extract H.264 Annex B bitstream from MP4 */
#include "../src/mp4_demux.h"
#include "../src/utils.h"
#include <stdlib.h>

static const uint8_t start_code[] = {0,0,0,1};

int main(int argc, char **argv) {
    if (argc < 3) { fprintf(stderr, "usage: %s <mp4> <out.h264>\n", argv[0]); return 1; }
    mp4_demux_t d;
    if (mp4_open(&d, argv[1]) < 0) return 1;
    if (d.video_idx < 0) { ERR("no video track"); mp4_close(&d); return 1; }

    FILE *out = fopen(argv[2], "wb");
    if (!out) { ERR("cannot open %s", argv[2]); mp4_close(&d); return 1; }

    mp4_track_t *t = &d.tracks[d.video_idx];

    /* write SPS and PPS with start codes */
    fwrite(start_code, 1, 4, out);
    fwrite(t->sps, 1, t->sps_len, out);
    fwrite(start_code, 1, 4, out);
    fwrite(t->pps, 1, t->pps_len, out);

    /* extract each sample, convert length-prefix NALUs to Annex B */
    uint8_t *buf = malloc(1024 * 1024);
    for (uint32_t i = 0; i < t->sample_count; i++) {
        int n = mp4_read_sample(&d, d.video_idx, i, buf, 1024 * 1024);
        if (n < 0) { ERR("read sample %u failed", i); break; }
        uint8_t *p = buf, *end = buf + n;
        while (p + t->nalu_len_size <= end) {
            uint32_t nalu_len = (t->nalu_len_size == 4) ? rd32(p) :
                                (t->nalu_len_size == 2) ? rd16(p) : *p;
            p += t->nalu_len_size;
            if (nalu_len == 0 || p + nalu_len > end) break;
            fwrite(start_code, 1, 4, out);
            fwrite(p, 1, nalu_len, out);
            p += nalu_len;
        }
    }

    free(buf);
    fclose(out);
    mp4_close(&d);
    LOG("wrote %s", argv[2]);
    return 0;
}
