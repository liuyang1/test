#ifndef MP4_DEMUX_H
#define MP4_DEMUX_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint64_t offset;
    uint32_t size;
    uint64_t dts;
    uint64_t pts;
    int      is_sync;
} mp4_sample_t;

typedef struct {
    uint32_t      track_id;
    uint32_t      timescale;
    uint32_t      codec;
    uint32_t      sample_count;
    mp4_sample_t *samples;

    uint8_t *sps;  uint16_t sps_len;
    uint8_t *pps;  uint16_t pps_len;
    uint8_t  nalu_len_size;

    uint8_t *asc;  uint16_t asc_len;
    uint32_t sample_rate;
    uint16_t channels;
    uint8_t  profile;
    uint8_t  freq_index;
    uint8_t  chan_config;
} mp4_track_t;

typedef struct {
    FILE        *fp;
    uint32_t     track_count;
    mp4_track_t  tracks[8];
    int          video_idx;
    int          audio_idx;
} mp4_demux_t;

int  mp4_open(mp4_demux_t *d, const char *path);
int  mp4_read_sample(mp4_demux_t *d, int track_idx,
                     uint32_t sample_idx, uint8_t *buf, uint32_t buf_size);
void mp4_close(mp4_demux_t *d);
void mp4_dump(const mp4_demux_t *d);

#endif
