#include "mp4_demux.h"
#include "rtsp_server.h"
#include "utils.h"
#include <signal.h>
#include <stdlib.h>

static rtsp_server_t server;

static void sighandler(int sig) {
    (void)sig;
    rtsp_server_stop(&server);
}

int main(int argc, char **argv) {
    const char *mp4_path = "tool/input.mp4";
    uint16_t port = 8554;

    if (argc >= 2) mp4_path = argv[1];
    if (argc >= 3) port = atoi(argv[2]);

    mp4_demux_t demux;
    if (mp4_open(&demux, mp4_path) < 0) return 1;
    mp4_dump(&demux);

    signal(SIGINT, sighandler);
    signal(SIGTERM, sighandler);

    if (rtsp_server_init(&server, port, &demux) < 0) {
        mp4_close(&demux);
        return 1;
    }

    rtsp_server_run(&server);

    mp4_close(&demux);
    return 0;
}
