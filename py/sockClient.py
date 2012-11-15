from socket import *
host = 'localhost'
port = 2000
sock = socket(AF_INET,SOCK_STREAM)
sock.connect((host,port))
while 1:
	try:
		str=raw_input(">>")
		if str=='q' or str=='quit':
			sock.close()
			break
		sock.send(str)
		data = sock.recv(1024)
		print data
	except:
		sock.close()
