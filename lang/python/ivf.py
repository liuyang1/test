import ctypes

tab = "  "


def showFields(obj, name, prefix=tab):
    s = name
    w = max([len(i) for i, _ in obj._fields_])
    for k, _ in obj._fields_:
        s += "\n%s%s: %s" % (prefix, k.ljust(w), getattr(obj, k))
    return s


class IvfHeader(ctypes.LittleEndianStructure):
    _fields_ = [
        ("signature", ctypes.c_char * 4),
        ("version", ctypes.c_ushort),
        ("lenght", ctypes.c_ushort),
        ("fourcc", ctypes.c_char * 4),
        ("width", ctypes.c_ushort),
        ("height", ctypes.c_ushort),
        ("framerate", ctypes.c_uint),
        ("timescale", ctypes.c_uint),
        ("numFrames", ctypes.c_uint),
        ("reserved", ctypes.c_uint),
    ]

    def __str__(self):
        return showFields(self, "IVF Header")


class IvfFrmHeader(ctypes.LittleEndianStructure):
    # with out _pack_, this will as 64bit alignment
    _pack_ = 1
    _fields_ = [
        ("size", ctypes.c_uint32),
        ("timestamp", ctypes.c_uint64),
    ]

    def __str__(self):
        return showFields(self, "IVF Header")

HdrLen = ctypes.sizeof(IvfHeader)
FrmLen = ctypes.sizeof(IvfFrmHeader)


def parseIVF(fn):
    with open(fn, "rb") as fp:
        data = fp.read()
        hdr = IvfHeader.from_buffer_copy(data)
        data = data[HdrLen:]
        print hdr
        while len(data) > 0:
            frm = IvfFrmHeader.from_buffer_copy(data)
            print frm
            data = data[FrmLen + frm.size:]

if __name__ == "__main__":
    parseIVF("720x480-dvDesk_420_br2048_p0_IP.ivf")
