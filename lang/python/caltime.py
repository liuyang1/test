from socket import *
import time
import datetime
import struct
import os
import sys

TIME1970 = 2208988800L
# print time.mktime(time.localtime())
# print time.mktime(time.gmtime())
# print time.time()
print time.ctime(time.time())

#time_server = ('time.apple.com',123)
#time_server = ('time.sinectis.com.ar',123)
#time_server = ('time.service.uit.no',123)
time_server = ('pool.ntp.org', 123)


client = socket(AF_INET, SOCK_DGRAM)
data = '\x1b' + 47 * '\0'
client.sendto(data, time_server)
data, address = client.recvfrom(1024)
if data:
    print 'Response received from', address, '\n'
    t = struct.unpack('!12I', data)[10]
    if t == 0:
        raise 'invalid response'
    ct = time.ctime(t - TIME1970)
    print 'Current time = %s ' % ct
else:
    raise 'no data returned'
