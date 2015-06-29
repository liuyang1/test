lst = [1, 5, 27, 168, 1084, 6861,
        41269, 260445, 1639117]
# lst = [2600, 20825, 166650, 1333300, 10666600]

from math import log
for i in xrange(1, len(lst)):
    a = lst[i - 1]
    b = lst[i] + 0.0
    d = b / a
    print "%.2f, %.2f" % (d, log(d) / log(2))
