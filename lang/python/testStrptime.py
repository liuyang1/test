t = "09-17 03:02:39.770"
from datetime import datetime
print t
print datetime.strptime(t, '%m-%d %H:%M:%S.%f')
