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

    def __init__(self, nodemap, edgelst):
        self.nodes = nodemap
        self.edges = edgelst

    def dump(self, linkprefix):
        linksuffix = ".html"
        ret = ""
        ret += "nodes: ["
        # TODO
        # echarts not support LABEL, so have to use name as LABEL and key
        for link, name in self.nodes.iteritems():
            rlink = linkprefix + link + linksuffix
            ret += "{{name: '{0}', link: '{1}'}},\n".format(name, rlink)
        ret += "],\n"
        ret += "links: ["
        for s, t in self.edges:
            ret += "{{source: '{0}', target: '{1}'}},\n" \
                .format(self.nodes[s], self.nodes[t])
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
    g = bigraph(data.nodemap, data.edgelst)
    linkprefix = "http://home.ustc.edu.cn/~liuyang1/html/"
    ret = g.dump(linkprefix)
    applyTpl('sitemap.html.tpl', (('FAKEDATA', ret), ))
