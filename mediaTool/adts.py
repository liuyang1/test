import sys
from construct import *

def increment(x):
    return x + 1

adts_hdr = BitStruct (
        "sync word" / Const(0xfff, BitsInteger(12)),
        "mpeg version" / Enum(BitsInteger(1), mpeg2 = 0, mpeg4 = 1),
        "layer" / BitsInteger(2),
        "protection" / Enum (BitsInteger(1), no_crc = 1, crc = 0),
        "profile_1" / BitsInteger(2),
        "profile MPEG4 AOT" / Computed(this.profile_1 + 1),
        "mpeg4 sampling freq idx" / Enum(BitsInteger(4),
            freq_96000 = 0, freq_88200 = 1, freq_64000 = 2, freq_48000 = 3, freq_44100 = 4,
            freq_32000 = 5, freq_24000 = 6, freq_22050 = 7, freq_16000 = 8, freq_12000 = 9,
            freq_11025 = 10, freq_8000 = 11, freq_7350 = 12
            ),
        #"private bit" / Flag,
        Padding(1),
        "mpeg4 channel cfg" / BitsInteger(3),
        #"originality" / Flag,
        #"home" / Flag,
        #"copyrighted id bit" / Flag,
        #"copyrighted id start" / Flag,
        Padding(4),
        "frame length" / BitsInteger(13),
        "buffer fullness" / BitsInteger(11),
        "num_frames_1" / BitsInteger(2),
        "num_frames" / Computed(this.num_frames_1 + 1)
        )



fn = sys.argv[1]
with open(fn, "rb") as f:
    h = adts_hdr.parse_stream(f)
    print(h)
