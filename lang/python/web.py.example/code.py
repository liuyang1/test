import web

urls = ('/', 'index')

def getlog():
    with open("/tmp/ott.log") as f:
        return f.read()

class index:

    def GET(self):
        return getlog()

if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()
