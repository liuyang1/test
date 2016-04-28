#! /usr/bin/env python2
"""
for fuzzy test on VP9
load data, mess on a little of data, then rewrite to new file.
"""
import copy


def load(fn):
    with open(fn, "rb") as fp:
        data = fp.read()
        return bytearray(data)


def save(fn, data):
    with open(fn, "wb") as fp:
        return fp.write(data)


def fuzzy(data, byte, begin, end):
    """
    overwrite data[begin:end] <= bytes ...
    """
    ndata = copy.deepcopy(data)
    for i in range(begin, end):
        ndata[i] = byte
    return ndata


def fuzzy_width(fn, start, step, num, width, byte):
    """
    generate NUM fuzzy test files,
        which overwrite by WIDTH of BYTE.
        from START with STEP
    """
    data = load(fn)
    stop = start + num * step
    for begin in range(start, stop, step):
        print begin, stop
        end = begin + width
        ndata = fuzzy(data, byte, begin, end)
        nfn = "%s.%#2x.%#x.%#x.fuz" % (fn, byte, begin, end)
        save(nfn, ndata)


if __name__ == "__main__":
    filename = "vp90_2_22_resize_fp_tiles_4_8.vp9.ivf"
    fuzzy_width(filename, 0xbbacc, 100, 2, 32, 0xff)
