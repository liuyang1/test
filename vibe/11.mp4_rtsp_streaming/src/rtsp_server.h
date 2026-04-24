#ifndef RTSP_SERVER_H
#define RTSP_SERVER_H

#include "mp4_demux.h"
#include "rtp_send.h"

typedef struct {
    char          session_id[16];
    int           tcp_fd;
    rtp_channel_t video_rtp;
    rtp_channel_t audio_rtp;
    int           video_setup;
    int           audio_setup;
    int           playing;
} rtsp_session_t;

typedef struct {
    int            listen_fd;
    uint16_t       port;
    mp4_demux_t   *demux;
    rtsp_session_t session;
    volatile int   running;
} rtsp_server_t;

int  rtsp_server_init(rtsp_server_t *srv, uint16_t port, mp4_demux_t *demux);
int  rtsp_server_run(rtsp_server_t *srv);
void rtsp_server_stop(rtsp_server_t *srv);

#endif
