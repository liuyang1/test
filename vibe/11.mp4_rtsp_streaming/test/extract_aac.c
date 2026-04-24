/* Extract AAC with ADTS headers from MP4 */
#include "../src/mp4_demux.h"
#include "../src/utils.h"
#include <stdlib.h>

static void write_adts(FILE *out, uint8_t profile, uint8_t freq_idx,
                       uint8_t chan_cfg, uint32_t frame_len) {
    uint32_t len = frame_len + 7; /* ADTS header is 7 bytes (no CRC) */
    uint8_t hdr[7];
    hdr[0] = 0xFF;
    hdr[1] = 0xF1;                                    /* MPEG-4, no CRC */
    hdr[2] = (profile << 6) | (freq_idx << 2) | (chan_cfg >> 2);
    hdr[3] = ((chan_cfg & 3) << 6) | (len >> 11);
    hdr[4] = (len >> 3) & 0xFF;
    hdr[5] = ((len & 7) << 5) | 0x1F;
    hdr[6] = 0xFC;
    fwrite(hdr, 1, 7, out);
}

int main(int argc, char **argv) {
    if (argc < 3) { fprintf(stderr, "usage: %s <mp4> <out.aac>\n", argv[0]); return 1; }
    mp4_demux_t d;
    if (mp4_open(&d, argv[1]) < 0) return 1;
    if (d.audio_idx < 0) { ERR("no audio track"); mp4_close(&d); return 1; }

    FILE *out = fopen(argv[2], "wb");
    if (!out) { ERR("cannot open %s", argv[2]); mp4_close(&d); return 1; }

    mp4_track_t *t = &d.tracks[d.audio_idx];
    uint8_t *buf = malloc(64 * 1024);

    for (uint32_t i = 0; i < t->sample_count; i++) {
        int n = mp4_read_sample(&d, d.audio_idx, i, buf, 64 * 1024);
        if (n < 0) { ERR("read sample %u failed", i); break; }
        write_adts(out, t->profile, t->freq_index, t->chan_config, n);
        fwrite(buf, 1, n, out);
    }

    free(buf);
    fclose(out);
    mp4_close(&d);
    LOG("wrote %s", argv[2]);
    return 0;
}
