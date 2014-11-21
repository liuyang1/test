import web
import os

urls = ('/', 'index')
logfn = '/tmp/log'

template = """
<html>
<head><script language="javascript" type="text/javascript">
var xmlHttp = new XMLHttpRequest();
xmlHttp.open("GET", "http://127.0.0.1:8080", true);
xmlHttp.onreadystatechange = updatePage;
xmlhttp.send(null);
</script></head>
<div id="statusRefresh"><table>
%s
</table>
</div>
</html>
"""

class index:

    def GET(self):
        cmd = 'tail -n 50 %s' % (logfn)
        text = os.popen(cmd).readlines()
        ret = template % ("".join(["<tr><td>%s</td></tr>\n" % l for l in text]))
        return ret

if __name__ == "__main__":
    app = web.application(urls, globals())
    app.run()
