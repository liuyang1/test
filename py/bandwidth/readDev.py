import time
while 1:
	fp=open("/proc/net/dev")
	for line in fp.readlines():
		line=line.split()
		if line[0].startswith('eth'):
			bytecnt=int(line[1])+int(line[9])
			pktcnt=int(line[2])+int(line[10])
			#print line[0]," bytecnt:",bytecnt,"\tpktcnt :",pktcnt
			print "[%s] if %s %12d B %12d pkt\r"%(time.ctime(),line[0],bytecnt,pktcnt),
	time.sleep(1)
