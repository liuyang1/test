#include "rtp_packet.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

void rtp_pkt_list_init(rtp_pkt_list_t *l) {
    memset(l, 0, sizeof(*l));
}

void rtp_pkt_list_free(rtp_pkt_list_t *l) {
    free(l->pkts);
    memset(l, 0, sizeof(*l));
}

static rtp_pkt_t *list_alloc(rtp_pkt_list_t *l) {
    if (l->count >= l->cap) {
        l->cap = l->cap ? l->cap * 2 : 16;
        l->pkts = realloc(l->pkts, l->cap * sizeof(rtp_pkt_t));
    }
    return &l->pkts[l->count++];
}

/* Write 12-byte RTP header */
static void rtp_header(uint8_t *h, int marker, uint8_t pt,
                       uint16_t seq, uint32_t ts, uint32_t ssrc) {
    h[0] = 0x80;                          /* V=2, P=0, X=0, CC=0 */
    h[1] = (marker ? 0x80 : 0) | (pt & 0x7f);
    wr16(h + 2, seq);
    wr32(h + 4, ts);
    wr32(h + 8, ssrc);
}

int rtp_packetize_h264(rtp_pkt_list_t *out, const uint8_t *nalu,
                       uint32_t nalu_len, uint32_t ts,
                       uint16_t *seq, uint32_t ssrc, int marker) {
    uint8_t pt = 96; /* dynamic payload type */

    if (nalu_len <= RTP_MAX_PKT_SIZE) {
        /* Single NALU packet */
        rtp_pkt_t *p = list_alloc(out);
        rtp_header(p->data, marker, pt, (*seq)++, ts, ssrc);
        memcpy(p->data + 12, nalu, nalu_len);
        p->len = 12 + nalu_len;
    } else {
        /* FU-A fragmentation */
        uint8_t nalu_type = nalu[0] & 0x1f;
        uint8_t nri = nalu[0] & 0x60;
        const uint8_t *payload = nalu + 1;
        uint32_t remaining = nalu_len - 1;
        int first = 1;

        while (remaining > 0) {
            uint32_t frag_size = remaining;
            int last = 1;
            if (frag_size > RTP_MAX_PKT_SIZE - 2) {
                frag_size = RTP_MAX_PKT_SIZE - 2;
                last = 0;
            }

            rtp_pkt_t *p = list_alloc(out);
            rtp_header(p->data, last && marker, pt, (*seq)++, ts, ssrc);

            /* FU indicator */
            p->data[12] = nri | 28; /* type = 28 (FU-A) */
            /* FU header */
            p->data[13] = (first ? 0x80 : 0) | (last ? 0x40 : 0) | nalu_type;

            memcpy(p->data + 14, payload, frag_size);
            p->len = 14 + frag_size;

            payload += frag_size;
            remaining -= frag_size;
            first = 0;
        }
    }
    return 0;
}

int rtp_packetize_aac(rtp_pkt_list_t *out, const uint8_t *frame,
                      uint32_t frame_len, uint32_t ts,
                      uint16_t *seq, uint32_t ssrc) {
    uint8_t pt = 97; /* dynamic payload type for audio */

    rtp_pkt_t *p = list_alloc(out);
    rtp_header(p->data, 1, pt, (*seq)++, ts, ssrc);

    /* AU header section (RFC 3640):
       2 bytes: AU-headers-length in bits (= 16 for one AU header)
       2 bytes: AU header (13-bit AU-size + 3-bit AU-Index) */
    p->data[12] = 0x00;
    p->data[13] = 0x10;  /* 16 bits */
    p->data[14] = (frame_len >> 5) & 0xff;
    p->data[15] = (frame_len & 0x1f) << 3;

    memcpy(p->data + 16, frame, frame_len);
    p->len = 16 + frame_len;
    return 0;
}
