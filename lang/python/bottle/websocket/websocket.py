import time
from bottle import request, Bottle, abort
app = Bottle()

@app.route('/websocket')
def handle_websocket():
    wsock = request.environ.get('wsgi.websocket')
    if not wsock:
        abort(400, 'Expected WebSocket request.')

    message = wsock.receive()
    cnt = 0
    while True:
        try:
            print "hit", cnt
            message = "no %d" % (cnt)
            wsock.send("Your message was: %r" % message)
            time.sleep(3)
            cnt += 1
        except WebSocketError:
            break

from gevent.pywsgi import WSGIServer
from geventwebsocket import WebSocketError
from geventwebsocket.handler import WebSocketHandler
server = WSGIServer(("0.0.0.0", 8080), app,
                    handler_class=WebSocketHandler)
server.serve_forever()
