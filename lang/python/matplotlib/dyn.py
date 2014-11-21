import matplotlib.pyplot as p
from scipy import eye
import time
for x in xrange(3, 7):
    p.imshow(eye(x))
    p.show(block=False)
    time.sleep(3)
    print x
