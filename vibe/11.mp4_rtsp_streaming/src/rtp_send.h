#ifndef RTP_SEND_H
#define RTP_SEND_H

#include "rtp_packet.h"
#include <netinet/in.h>

typedef struct {
    int      sockfd;
    uint16_t server_port;
    uint16_t client_port;
    uint32_t ssrc;
    uint16_t seq;
    struct sockaddr_in client_addr;
} rtp_channel_t;

int  rtp_channel_open(rtp_channel_t *ch, uint16_t server_port);
void rtp_channel_set_dest(rtp_channel_t *ch, uint32_t client_ip, uint16_t client_port);
int  rtp_channel_send(rtp_channel_t *ch, const rtp_pkt_list_t *list);
void rtp_channel_close(rtp_channel_t *ch);

#endif
