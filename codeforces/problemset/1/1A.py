line = input()
d = [int(a) for a in line.split()]
import math
print(math.ceil(d[0] / d[2]) * math.ceil(d[1] / d[2]))
