#ifndef RTP_PACKET_H
#define RTP_PACKET_H

#include <stdint.h>

#define RTP_MAX_PKT_SIZE 1400  /* MTU-safe payload size */

typedef struct {
    uint8_t  data[RTP_MAX_PKT_SIZE + 12]; /* 12-byte RTP header + payload */
    uint16_t len;
} rtp_pkt_t;

typedef struct {
    rtp_pkt_t *pkts;
    int        count;
    int        cap;
} rtp_pkt_list_t;

/* H.264 RTP packetization (RFC 6184): single NALU or FU-A fragmentation.
   nalu points to raw NALU data (without start code or length prefix).
   Caller manages seq (incremented per packet) and provides ssrc/ts. */
int rtp_packetize_h264(rtp_pkt_list_t *out, const uint8_t *nalu,
                       uint32_t nalu_len, uint32_t ts,
                       uint16_t *seq, uint32_t ssrc, int marker);

/* AAC RTP packetization (RFC 3640): AU header section + single frame.
   frame points to raw AAC frame data (no ADTS header). */
int rtp_packetize_aac(rtp_pkt_list_t *out, const uint8_t *frame,
                      uint32_t frame_len, uint32_t ts,
                      uint16_t *seq, uint32_t ssrc);

void rtp_pkt_list_init(rtp_pkt_list_t *l);
void rtp_pkt_list_free(rtp_pkt_list_t *l);

#endif
