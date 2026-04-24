#include "../src/mp4_demux.h"
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "usage: %s <mp4>\n", argv[0]); return 1; }
    mp4_demux_t d;
    if (mp4_open(&d, argv[1]) < 0) return 1;
    mp4_dump(&d);

    /* print all samples for each track */
    for (uint32_t i = 0; i < d.track_count; i++) {
        mp4_track_t *t = &d.tracks[i];
        char cc[5] = {t->codec>>24, t->codec>>16, t->codec>>8, t->codec, 0};
        printf("=== track %u (%s) samples=%u timescale=%u ===\n",
               i, cc, t->sample_count, t->timescale);
        for (uint32_t j = 0; j < t->sample_count; j++) {
            mp4_sample_t *s = &t->samples[j];
            printf("  [%3u] off=%-8lu size=%-6u dts=%-8lu pts=%-8lu sync=%d\n",
                   j, s->offset, s->size, s->dts, s->pts, s->is_sync);
        }
    }
    mp4_close(&d);
    return 0;
}
