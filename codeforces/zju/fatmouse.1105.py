# It visited both sides of each street.
# So it have to travel with 20km/h for two directions.
# 1. It's kind of Chinese Postman Problem (CPP) with two directions.
# 2. When G is Euler Graph, then any travel in graph is optimal.
# 3. Euler Graph: Iff all vertex have even degree,
# As every street have two directions, so it must be even sides.
# so it's Euler Graph, The length of travel path is 2 * SUM(len(street))

# This problem haven't describle input & output format in details
# - input number is float/double, but not integer
# - output format is "%d:%02d", must have prefix 0 for mm
# - it support multiple test cases, until reading failure


import math


def dist2(ax, ay, bx, by):
    return math.sqrt((ax - bx) ** 2 + (ay - by) ** 2)


while True:
    try:
        _ = raw_input()  # this is origin vertex, we could ignore it.
    except EOFError:
        break
    length = 0
    while True:
        l = raw_input()
        if l == 'java':
            break
        l = [float(i) for i in l.split()]
        length += dist2(l[0], l[1], l[2], l[3])
    t = (2 * length) / 20000. * 60
    t = round(t)
    hh, mm = t / 60, t % 60
    print "%d:%02d" % (hh, mm)
