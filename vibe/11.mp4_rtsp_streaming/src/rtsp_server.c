#define _POSIX_C_SOURCE 200112L
#include "rtsp_server.h"
#include "rtp_packet.h"
#include "sdp.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define RTSP_BUF_SIZE 4096
#define RTP_VIDEO_PORT 50000
#define RTP_AUDIO_PORT 50002

/* ---- RTSP request parsing ---- */
typedef struct {
    char method[16];
    char url[256];
    int  cseq;
    char transport[256];
    char session[16];
} rtsp_request_t;

static int parse_request(const char *buf, rtsp_request_t *req) {
    memset(req, 0, sizeof(*req));
    sscanf(buf, "%15s %255s", req->method, req->url);

    const char *p = strstr(buf, "CSeq:");
    if (p) req->cseq = atoi(p + 5);

    p = strstr(buf, "Transport:");
    if (p) {
        p += 10;
        while (*p == ' ') p++;
        int i = 0;
        while (*p && *p != '\r' && *p != '\n' && i < 255)
            req->transport[i++] = *p++;
        req->transport[i] = 0;
    }

    p = strstr(buf, "Session:");
    if (p) {
        p += 8;
        while (*p == ' ') p++;
        int i = 0;
        while (*p && *p != '\r' && *p != '\n' && *p != ';' && i < 15)
            req->session[i++] = *p++;
        req->session[i] = 0;
    }
    return 0;
}

static int get_client_port(const char *transport) {
    const char *p = strstr(transport, "client_port=");
    if (!p) return -1;
    return atoi(p + 12);
}

static int get_track_id(const char *url) {
    const char *p = strstr(url, "trackID=");
    if (!p) p = strstr(url, "track");
    if (!p) return -1;
    while (*p && (*p < '0' || *p > '9')) p++;
    return atoi(p);
}

/* ---- RTSP response helpers ---- */
static int resp_options(char *buf, int size, int cseq) {
    return snprintf(buf, size,
        "RTSP/1.0 200 OK\r\n"
        "CSeq: %d\r\n"
        "Public: OPTIONS, DESCRIBE, SETUP, PLAY, TEARDOWN\r\n\r\n", cseq);
}

static int resp_describe(char *buf, int size, int cseq,
                         const mp4_demux_t *d, const char *url, const char *server_ip) {
    char sdp[2048];
    int sdp_len = sdp_generate(sdp, sizeof(sdp), d, server_ip);
    return snprintf(buf, size,
        "RTSP/1.0 200 OK\r\n"
        "CSeq: %d\r\n"
        "Content-Base: %s\r\n"
        "Content-Type: application/sdp\r\n"
        "Content-Length: %d\r\n\r\n%s", cseq, url, sdp_len, sdp);
}

static int resp_setup(char *buf, int size, int cseq, const char *session,
                      int client_port, int server_port) {
    return snprintf(buf, size,
        "RTSP/1.0 200 OK\r\n"
        "CSeq: %d\r\n"
        "Session: %s\r\n"
        "Transport: RTP/AVP;unicast;client_port=%d-%d;server_port=%d-%d\r\n\r\n",
        cseq, session, client_port, client_port + 1,
        server_port, server_port + 1);
}

static int resp_play(char *buf, int size, int cseq, const char *session) {
    return snprintf(buf, size,
        "RTSP/1.0 200 OK\r\n"
        "CSeq: %d\r\n"
        "Session: %s\r\n"
        "Range: npt=0.000-\r\n\r\n", cseq, session);
}

static int resp_teardown(char *buf, int size, int cseq) {
    return snprintf(buf, size,
        "RTSP/1.0 200 OK\r\n"
        "CSeq: %d\r\n\r\n", cseq);
}

/* ---- streaming loop ---- */
static void stream_loop(rtsp_server_t *srv) {
    mp4_demux_t *d = srv->demux;
    rtsp_session_t *s = &srv->session;
    mp4_track_t *vt = (d->video_idx >= 0) ? &d->tracks[d->video_idx] : NULL;
    mp4_track_t *at = (d->audio_idx >= 0) ? &d->tracks[d->audio_idx] : NULL;

    uint32_t vi = 0, ai = 0; /* current sample index */
    uint8_t *buf = malloc(1024 * 1024);
    rtp_pkt_list_t pkt_list;
    rtp_pkt_list_init(&pkt_list);

    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);

    LOG("streaming started");

    while (s->playing && srv->running) {
        /* Determine next event: video or audio sample, whichever has earlier DTS */
        int send_video = 0, send_audio = 0;
        uint64_t v_dts = UINT64_MAX, a_dts = UINT64_MAX;
        double v_time = 1e30, a_time = 1e30;

        if (vt && vi < vt->sample_count) {
            v_dts = vt->samples[vi].dts;
            v_time = (double)v_dts / vt->timescale;
        }
        if (at && ai < at->sample_count) {
            a_dts = at->samples[ai].dts;
            a_time = (double)a_dts / at->timescale;
        }

        if (v_time >= 1e30 && a_time >= 1e30) break; /* done */

        if (v_time <= a_time) send_video = 1;
        else send_audio = 1;

        /* Wait until it's time to send */
        double send_time = send_video ? v_time : a_time;
        struct timespec target;
        target.tv_sec = start.tv_sec + (long)send_time;
        target.tv_nsec = start.tv_nsec + (long)((send_time - (long)send_time) * 1e9);
        if (target.tv_nsec >= 1000000000L) {
            target.tv_sec++;
            target.tv_nsec -= 1000000000L;
        }
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &target, NULL);

        /* Check if client sent TEARDOWN (non-blocking read on TCP) */
        {
            char tmp[RTSP_BUF_SIZE];
            struct timeval tv = {0, 0};
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(s->tcp_fd, &fds);
            if (select(s->tcp_fd + 1, &fds, NULL, NULL, &tv) > 0) {
                int n = recv(s->tcp_fd, tmp, sizeof(tmp) - 1, 0);
                if (n <= 0) { s->playing = 0; break; }
                tmp[n] = 0;
                if (strstr(tmp, "TEARDOWN")) {
                    rtsp_request_t req;
                    parse_request(tmp, &req);
                    char resp[256];
                    int rlen = resp_teardown(resp, sizeof(resp), req.cseq);
                    send(s->tcp_fd, resp, rlen, 0);
                    s->playing = 0;
                    break;
                }
            }
        }

        if (send_video && s->video_setup) {
            int n = mp4_read_sample(d, d->video_idx, vi, buf, 1024 * 1024);
            if (n > 0) {
                /* Convert RTP timestamp: PTS in track timescale → 90kHz */
                uint32_t rtp_ts = (uint32_t)((uint64_t)vt->samples[vi].pts * 90000 / vt->timescale);

                /* Parse NALUs from sample */
                uint8_t *p = buf, *end = buf + n;
                while (p + vt->nalu_len_size <= end) {
                    uint32_t nalu_len = (vt->nalu_len_size == 4) ? rd32(p) :
                                        (vt->nalu_len_size == 2) ? rd16(p) : *p;
                    p += vt->nalu_len_size;
                    if (nalu_len == 0 || p + nalu_len > end) break;

                    int is_last_nalu = (p + nalu_len >= end) ||
                                       (p + nalu_len + vt->nalu_len_size > end);
                    pkt_list.count = 0;
                    rtp_packetize_h264(&pkt_list, p, nalu_len, rtp_ts,
                                       &s->video_rtp.seq, s->video_rtp.ssrc,
                                       is_last_nalu);
                    rtp_channel_send(&s->video_rtp, &pkt_list);
                    p += nalu_len;
                }
            }
            vi++;
        }

        if (send_audio && s->audio_setup) {
            int n = mp4_read_sample(d, d->audio_idx, ai, buf, 1024 * 1024);
            if (n > 0) {
                uint32_t rtp_ts = (uint32_t)((uint64_t)at->samples[ai].pts * at->sample_rate / at->timescale);
                pkt_list.count = 0;
                rtp_packetize_aac(&pkt_list, buf, n, rtp_ts,
                                  &s->audio_rtp.seq, s->audio_rtp.ssrc);
                rtp_channel_send(&s->audio_rtp, &pkt_list);
            }
            ai++;
        }
    }

    free(buf);
    rtp_pkt_list_free(&pkt_list);
    LOG("streaming ended (video: %u/%u, audio: %u/%u)",
        vi, vt ? vt->sample_count : 0, ai, at ? at->sample_count : 0);
}

/* ---- handle one RTSP client ---- */
static void handle_client(rtsp_server_t *srv, int client_fd,
                          struct sockaddr_in *client_addr) {
    rtsp_session_t *s = &srv->session;
    memset(s, 0, sizeof(*s));
    s->tcp_fd = client_fd;
    snprintf(s->session_id, sizeof(s->session_id), "%08X", (unsigned)time(NULL));
    s->video_rtp.ssrc = 0x12345678;
    s->audio_rtp.ssrc = 0x12345679;
    s->video_rtp.sockfd = -1;
    s->audio_rtp.sockfd = -1;

    char *server_ip = inet_ntoa(((struct sockaddr_in *)client_addr)->sin_addr);
    /* Use the server's own address that the client connected to */
    {
        struct sockaddr_in local;
        socklen_t len = sizeof(local);
        getsockname(client_fd, (struct sockaddr *)&local, &len);
        server_ip = inet_ntoa(local.sin_addr);
    }
    char server_ip_buf[32];
    strncpy(server_ip_buf, server_ip, sizeof(server_ip_buf) - 1);

    LOG("client connected from %s:%d",
        inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));

    char buf[RTSP_BUF_SIZE];
    while (srv->running) {
        int n = recv(client_fd, buf, sizeof(buf) - 1, 0);
        if (n <= 0) break;
        buf[n] = 0;

        rtsp_request_t req;
        parse_request(buf, &req);
        LOG("RTSP %s %s CSeq=%d", req.method, req.url, req.cseq);

        char resp[4096];
        int rlen = 0;

        if (strcmp(req.method, "OPTIONS") == 0) {
            rlen = resp_options(resp, sizeof(resp), req.cseq);
        } else if (strcmp(req.method, "DESCRIBE") == 0) {
            rlen = resp_describe(resp, sizeof(resp), req.cseq,
                                 srv->demux, req.url, server_ip_buf);
        } else if (strcmp(req.method, "SETUP") == 0) {
            int track_id = get_track_id(req.url);
            int client_port = get_client_port(req.transport);
            uint32_t cip = client_addr->sin_addr.s_addr;

            if (track_id == 0 && srv->demux->video_idx >= 0) {
                rtp_channel_open(&s->video_rtp, RTP_VIDEO_PORT);
                rtp_channel_set_dest(&s->video_rtp, cip, client_port);
                s->video_setup = 1;
                rlen = resp_setup(resp, sizeof(resp), req.cseq,
                                  s->session_id, client_port, RTP_VIDEO_PORT);
            } else if (track_id == 1 && srv->demux->audio_idx >= 0) {
                rtp_channel_open(&s->audio_rtp, RTP_AUDIO_PORT);
                rtp_channel_set_dest(&s->audio_rtp, cip, client_port);
                s->audio_setup = 1;
                rlen = resp_setup(resp, sizeof(resp), req.cseq,
                                  s->session_id, client_port, RTP_AUDIO_PORT);
            } else {
                rlen = snprintf(resp, sizeof(resp),
                    "RTSP/1.0 404 Not Found\r\nCSeq: %d\r\n\r\n", req.cseq);
            }
        } else if (strcmp(req.method, "PLAY") == 0) {
            rlen = resp_play(resp, sizeof(resp), req.cseq, s->session_id);
            send(client_fd, resp, rlen, 0);
            s->playing = 1;
            stream_loop(srv);
            break; /* stream done, close connection */
        } else if (strcmp(req.method, "TEARDOWN") == 0) {
            rlen = resp_teardown(resp, sizeof(resp), req.cseq);
            send(client_fd, resp, rlen, 0);
            break;
        } else {
            rlen = snprintf(resp, sizeof(resp),
                "RTSP/1.0 405 Method Not Allowed\r\nCSeq: %d\r\n\r\n", req.cseq);
        }

        if (rlen > 0) send(client_fd, resp, rlen, 0);
    }

    /* cleanup */
    rtp_channel_close(&s->video_rtp);
    rtp_channel_close(&s->audio_rtp);
    close(client_fd);
    LOG("client disconnected");
}

/* ---- public API ---- */
int rtsp_server_init(rtsp_server_t *srv, uint16_t port, mp4_demux_t *demux) {
    memset(srv, 0, sizeof(*srv));
    srv->port = port;
    srv->demux = demux;
    srv->running = 1;

    srv->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (srv->listen_fd < 0) { ERR("socket failed"); return -1; }

    int opt = 1;
    setsockopt(srv->listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(srv->listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        ERR("bind port %u failed: %s", port, strerror(errno));
        close(srv->listen_fd);
        return -1;
    }
    if (listen(srv->listen_fd, 1) < 0) {
        ERR("listen failed");
        close(srv->listen_fd);
        return -1;
    }

    LOG("RTSP server listening on rtsp://0.0.0.0:%u/stream", port);
    return 0;
}

int rtsp_server_run(rtsp_server_t *srv) {
    signal(SIGPIPE, SIG_IGN);

    while (srv->running) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = accept(srv->listen_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd < 0) {
            if (errno == EINTR) continue;
            ERR("accept failed");
            break;
        }
        handle_client(srv, client_fd, &client_addr);
    }
    return 0;
}

void rtsp_server_stop(rtsp_server_t *srv) {
    srv->running = 0;
    close(srv->listen_fd);
}
