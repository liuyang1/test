#include "sdp.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

/* Base64 encode for SPS/PPS/ASC in SDP fmtp */
static const char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static int base64_encode(char *out, int out_size, const uint8_t *in, int in_len) {
    int o = 0;
    for (int i = 0; i < in_len; i += 3) {
        if (o + 4 >= out_size) return -1;
        uint32_t v = in[i] << 16;
        if (i + 1 < in_len) v |= in[i+1] << 8;
        if (i + 2 < in_len) v |= in[i+2];
        out[o++] = b64[(v >> 18) & 0x3f];
        out[o++] = b64[(v >> 12) & 0x3f];
        out[o++] = (i + 1 < in_len) ? b64[(v >> 6) & 0x3f] : '=';
        out[o++] = (i + 2 < in_len) ? b64[v & 0x3f] : '=';
    }
    out[o] = 0;
    return o;
}

int sdp_generate(char *buf, int buf_size, const mp4_demux_t *d,
                 const char *server_ip) {
    int n = 0;
    n += snprintf(buf + n, buf_size - n,
        "v=0\r\n"
        "o=- 0 0 IN IP4 %s\r\n"
        "s=RTSP Stream\r\n"
        "c=IN IP4 0.0.0.0\r\n"
        "t=0 0\r\n",
        server_ip);

    /* Video track */
    if (d->video_idx >= 0) {
        const mp4_track_t *t = &d->tracks[d->video_idx];
        char sps_b64[256], pps_b64[256];
        base64_encode(sps_b64, sizeof(sps_b64), t->sps, t->sps_len);
        base64_encode(pps_b64, sizeof(pps_b64), t->pps, t->pps_len);

        /* profile-level-id: 3 bytes from SPS (profile_idc, constraint_flags, level_idc) */
        char profile_level_id[7];
        snprintf(profile_level_id, sizeof(profile_level_id), "%02X%02X%02X",
                 t->sps[1], t->sps[2], t->sps[3]);

        n += snprintf(buf + n, buf_size - n,
            "m=video 0 RTP/AVP 96\r\n"
            "a=rtpmap:96 H264/90000\r\n"
            "a=fmtp:96 packetization-mode=1;"
            "profile-level-id=%s;"
            "sprop-parameter-sets=%s,%s\r\n"
            "a=control:trackID=0\r\n",
            profile_level_id, sps_b64, pps_b64);
    }

    /* Audio track */
    if (d->audio_idx >= 0) {
        const mp4_track_t *t = &d->tracks[d->audio_idx];
        /* config= hex string of AudioSpecificConfig */
        char config_hex[64];
        for (int i = 0; i < t->asc_len && i * 2 + 2 < (int)sizeof(config_hex); i++)
            snprintf(config_hex + i * 2, 3, "%02X", t->asc[i]);

        n += snprintf(buf + n, buf_size - n,
            "m=audio 0 RTP/AVP 97\r\n"
            "a=rtpmap:97 MPEG4-GENERIC/%u/%u\r\n"
            "a=fmtp:97 streamtype=5;profile-level-id=1;"
            "mode=AAC-hbr;sizelength=13;indexlength=3;"
            "indexdeltalength=3;config=%s\r\n"
            "a=control:trackID=1\r\n",
            t->sample_rate, t->channels, config_hex);
    }

    return n;
}
