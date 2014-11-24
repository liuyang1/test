"""
generate vimwiki's sitemap.

Input:
    viwmiki dfsvw.py, generate data.lst which is a edge link
    echarts force mode, template file, sitemap.html.tpl
Output:
    write a html, sitemap.html

2014-11-24 00:08:32
"""
import data


class bigraph():

    def __init__(self):
        self.edges = []

    def addEdge(self, source, target):
        self.edges.append((source, target))

    def dump(self):
        nodes = set()
        for s, t in self.edges:
            nodes.add(s)
            nodes.add(t)
        ret = ""
        ret += "nodes: ["
        for n in nodes:
            ret += "\t{{name: '{0}'}},".format(n)
        ret += "],"
        ret += "links: ["
        for e in self.edges:
            ret += "\t{{source: '{0}', target: '{1}'}},".format(*e)
        ret += "]"
        return ret


def applyTpl(tpl, trans):
    f = open(tpl)
    content = f.read()
    for f, t in trans:
        nc = content.replace(f, t)
    fo = open("sitemap.html", "w")
    fo.write(nc)

if __name__ == "__main__":
    g = bigraph()
    for i in data.lst:
        g.addEdge(*i)
    ret = g.dump()
    applyTpl('sitemap.html.tpl', (('FAKEDATA', ret), ))
