import ctypes


class Point(ctypes.LittleEndianStructure):
    _fields_ = [
        ("x", ctypes.c_int32),
        ("y", ctypes.c_int32)
    ]

buffer = "\x01\x00\x00\x00\x02\x00\x00\x00"
point = Point.from_buffer_copy(buffer)
assert point.x == 1
assert point.y == 2

buffer = bytearray(ctypes.sizeof(Point))
assert buffer == bytearray("\x00\x00\x00\x00\x00\x00\x00\x00")
point = Point.from_buffer(buffer)
point.x = 1
point.y = 2
assert buffer == bytearray("\x01\x00\x00\x00\x02\x00\x00\x00")

point = Point(1, 2)
buffer = ctypes.string_at(ctypes.addressof(point), ctypes.sizeof(point))
assert buffer == "\x01\x00\x00\x00\x02\x00\x00\x00"
