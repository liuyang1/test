from gevent.server import StreamServer


def echo(socket, address):
    print 'connect from %s:%s' % address
    #socket.sendall('Welcome to echo server! Type quit to exit\r\n')
    fileobj = socket.makefile()
    while True:
        line = fileobj.readline()
        if not line:
            print 'client disconnected'
            break
        if line.strip().lower() == 'quit':
            print 'cleint quit'
            break
        print line
        fileobj.write('echo back ' + line)
        fileobj.flush()

if __name__ == "__main__":
    server = StreamServer(('', 6000), echo)
    print 'start echo server @ 6000'
    server.serve_forever()
