from gevent import monkey

monkey.patch_all()

import gevent
from bottle import route, run

teststr = """ <form action="form">
  First name: <input type="text" name="fname"><br>
  Last name: <input type="text" name="lname"><br>
  <input type="submit" value="Submit">
</form>"""


@route('/form')
def form():
    yield "queue"
    gevent.sleep(2)
    yield "received"


@route('/test')
def testpage():
    return teststr


@route('/stream')
def stream():
    yield '<p>%s</p>' % ('start')
    gevent.sleep(3)
    yield '<p>%s</p>' % ('middle')
    gevent.sleep(5)
    yield '<p>%s</p>' % ('end')

run(host='0.0.0.0', port=8080, server='gevent')
