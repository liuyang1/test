CC = gcc
CFLAGS = -Wall -Wextra -g -O2 -std=c99 -D_FILE_OFFSET_BITS=64
LDFLAGS = -lm

SRCDIR = src
TESTDIR = test
BUILDDIR = build

SERVER = $(BUILDDIR)/rtsp_server
DUMP_MP4 = $(BUILDDIR)/dump_mp4
EXTRACT_H264 = $(BUILDDIR)/extract_h264
EXTRACT_AAC = $(BUILDDIR)/extract_aac

SERVER_SRCS = $(SRCDIR)/main.c $(SRCDIR)/mp4_demux.c $(SRCDIR)/rtp_packet.c \
              $(SRCDIR)/sdp.c $(SRCDIR)/rtp_send.c $(SRCDIR)/rtsp_server.c

.PHONY: all clean tests server

all: server tests

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

server: $(SERVER)

$(SERVER): $(SERVER_SRCS) | $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) -o $@ $(SERVER_SRCS) $(LDFLAGS)

tests: $(DUMP_MP4) $(EXTRACT_H264) $(EXTRACT_AAC)

$(DUMP_MP4): $(TESTDIR)/dump_mp4.c $(SRCDIR)/mp4_demux.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) -o $@ $^ $(LDFLAGS)

$(EXTRACT_H264): $(TESTDIR)/extract_h264.c $(SRCDIR)/mp4_demux.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) -o $@ $^ $(LDFLAGS)

$(EXTRACT_AAC): $(TESTDIR)/extract_aac.c $(SRCDIR)/mp4_demux.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(BUILDDIR)
