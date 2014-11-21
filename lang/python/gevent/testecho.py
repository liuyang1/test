from socket import *


socket = socket(AF_INET, SOCK_STREAM)
socket.connect(('127.0.0.1', 6000))
while True:
    s = raw_input('>')
    if s == 'quit':
        break
    socket.send(s + '\r\n')
    print socket.recv(1500)
socket.close()
