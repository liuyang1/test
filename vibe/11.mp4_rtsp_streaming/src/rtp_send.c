#include "rtp_send.h"
#include "utils.h"
#include <unistd.h>
#include <string.h>

int rtp_channel_open(rtp_channel_t *ch, uint16_t server_port) {
    memset(ch, 0, sizeof(*ch));
    ch->server_port = server_port;
    ch->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (ch->sockfd < 0) { ERR("socket failed"); return -1; }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(server_port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(ch->sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        ERR("bind port %u failed", server_port);
        close(ch->sockfd);
        ch->sockfd = -1;
        return -1;
    }
    return 0;
}

void rtp_channel_set_dest(rtp_channel_t *ch, uint32_t client_ip, uint16_t client_port) {
    ch->client_port = client_port;
    ch->client_addr.sin_family = AF_INET;
    ch->client_addr.sin_port = htons(client_port);
    ch->client_addr.sin_addr.s_addr = client_ip;
}

int rtp_channel_send(rtp_channel_t *ch, const rtp_pkt_list_t *list) {
    for (int i = 0; i < list->count; i++) {
        const rtp_pkt_t *p = &list->pkts[i];
        ssize_t sent = sendto(ch->sockfd, p->data, p->len, 0,
                              (struct sockaddr *)&ch->client_addr,
                              sizeof(ch->client_addr));
        if (sent < 0) { ERR("sendto failed"); return -1; }
    }
    return 0;
}

void rtp_channel_close(rtp_channel_t *ch) {
    if (ch->sockfd >= 0) close(ch->sockfd);
    ch->sockfd = -1;
}
