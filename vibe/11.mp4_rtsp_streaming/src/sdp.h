#ifndef SDP_H
#define SDP_H

#include "mp4_demux.h"

/* Generate SDP description into buf. Returns length written, or -1 on error. */
int sdp_generate(char *buf, int buf_size, const mp4_demux_t *d,
                 const char *server_ip);

#endif
