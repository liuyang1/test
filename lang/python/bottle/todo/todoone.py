from bottle import route, run, debug, template, request
import sqlite3


def pretty_print_POST(req):
    """
    At this point it is completely built and ready
    to be fired; it is "prepared".

    However pay attention at the formatting used in 
    this function because it is programmed to be pretty 
    printed and may differ from the actual request.
    """
    print('{}\n{}\n{}\n\n{}'.format(
        '-----------START-----------',
        req.method + ' ' + req.url,
        '\n'.join('{}: {}'.format(k, v) for k, v in req.headers.items()),
        req.body,
    ))
    print ("----")
    print(req.body.getvalue())
    print ("----")

items = {1: 'first item', 2: 'second item'}

@route('/new')
def new_item():
    print request
    # req = request.prepare()
    pretty_print_POST(request)
    if request.GET.get('task').strip():
        new = request.GET.get('task', '').strip()
        conn = sqlite3.connect('todo.db')
        c = conn.cursor()
        c.execute('INSERT INTO todo (task,status) VALUES (?,?)', (new, 1))
        newid = c.lastrowid
        conn.commit()
        c.close()
        return '{id: %s}' % (newid)
    else:
        return "nothing"


@route('/')
def index():
    conn = sqlite3.connect('todo.db')
    c = conn.cursor()
    c.execute("SELECT id, task FROM todo WHERE status LIKE '1'")
    result = c.fetchall()
    c.close()
    output = template('todone', rows=result)
    return output

debug(True)
run(host='0.0.0.0', port=8080, reloader=True)
