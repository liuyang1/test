#! /usr/bin/env python2
import sys
import os.path
import subprocess


FNULL = open(os.devnull, 'w')


def usage(this):
    print "usage: %s [mediafile]"
    print "\ndepend on: [ffprobe, Python, matplotlib, pylab]"


def gen_cache(fn, cache):
    with open(cache, "w") as cache_fp:
        cmd = ['ffprobe', '-show_frames', '-print_format', 'compact',
               '-select_streams', 'v', fn]
        r = subprocess.check_call(cmd, stdout=cache_fp, stderr=FNULL)


def get_with_tag(line, head, tail):
    beg = line.index(head) + len(head)
    end = line.index(tail, beg)
    return line[beg:end]


def calc(data, fps):
    lst = []
    with open(data) as fp:
        lineno = 0
        for line in fp.read().split('\n'):
            if line == "":
                break
            lineno += 1
            # print line
            s = get_with_tag(line, "pkt_size=", "|")
            size = int(s)
            lst.append(size)
        # print lst

    win = fps
    win1 = int(round(win))

    bitrates = []
    for i in range(0, len(lst) - win1):
        s = sum(lst[i:i + win1])
        bitrate = s / (win1 / (win + 0.0))
        # print i, bitrate
        bitrates.append(bitrate)
    return bitrates


def gen_fps(fn):
    cmd = ['ffprobe', '-show_streams', '-print_format', 'compact',
           '-select_streams', 'v', '-show_entries', 'stream=avg_frame_rate',
           fn]
    out = subprocess.check_output(cmd, stderr=FNULL)
    # print out
    s = get_with_tag(out, 'avg_frame_rate=', '/')
    num = int(s)
    s = get_with_tag(out, "/", "|")
    den = int(s)
    return (num + 0.0) / den


def plot(bitrates, fps, fn):
    import matplotlib
    matplotlib.use('Agg')  # to avoid X dependency
    import matplotlib.pyplot as pl
    pl.style.use('ggplot')
    xs = [i / fps for i in range(len(bitrates))]
    pl.plot(xs, bitrates)
    pl.xlabel('time(seconds)')
    pl.ylabel("bitrate(Mbps)")
    pl.title("Peak Bitrate of %s" % (fn))
    pl.ylim(0)
    import pylab
    pylab.savefig(fn)


if __name__ == "__main__":
    if len(sys.argv) <= 1:
        usage()
        sys.exit()

    mediafile = sys.argv[1]
    cache = "%s.frames.cache" % (mediafile)
    bitrateFn = "%s.frames.png" % (mediafile)

    if not os.path.exists(cache):
        print "generate cache file: %s ..." % (cache)
        gen_cache(mediafile, cache)
        print "generate cache file: %s done"
    else:
        print "find cache file: %s" % (cache)

    fps = gen_fps(mediafile)
    # print fps
    bitrates = calc(cache, fps)
    bitrates = [8 * i / 1000 / 1000 for i in bitrates]

    m = max(bitrates)
    t = bitrates.index(m) / fps
    print "Max Bitrate: %.2fMbps" % (m)
    print "Time:        %.2fs" % (t)
    plot(bitrates, fps, bitrateFn)
