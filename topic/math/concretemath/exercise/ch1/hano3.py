import networkx as nx
import matplotlib.pyplot as plt
import sys
def renderDisks(xs):
    if len(xs) == 0:
        return "0"
    else:
        return "".join([str(x) for x in xs])

def renderSt(st):
    return "n" + "_".join([renderDisks(x) for x in st])

def labelSt(st):
    return ",".join([renderDisks(x) for x in st])

def renderG(g):
    n, sts, es = g[0], g[1], g[2]
    print("graph {")
    print("    rankdir=\"LR\";")
    print("    title=\"hano%d\";" % (n))
    for st in sts:
        print("    %s [label=\"%s\"];" % (renderSt(st), labelSt(st)))
    for e in es:
        print("    %s -- %s" % (renderSt(e[0]), renderSt(e[1])))
    print("}")


def renderG_file(g, fn):
    orignial_stdout = sys.stdout
    with open(fn, 'w') as f:
        sys.stdout = f
        renderG(g)
        sys.stdout = orignial_stdout

def buildNx(g):
    G = nx.Graph()
    n, sts, es = g[0], g[1], g[2]
    for st in sts:
        G.add_node(renderSt(st), label=labelSt(st))
    for e0, e1 in es:
        G.add_edge(renderSt(e0), renderSt(e1))
    return G

def renderNx(G):
    # pos = nx.spring_layout(G)
    pos = nx.kamada_kawai_layout(G)
    plt.figure(figsize=(10, 8))
    nx.draw(G, pos) #, with_labels=True,
    plt.show()

def G0():
    st = ([], [], [])
    g = []
    return (0, [st], g)

def xappend(lst, x):
    new = list(lst)
    new.append(x)
    return new

def nextg(g, flag=1):
    ng, sts, g = g[0], g[1], g[2]
    next_ng = ng + 1
    def inject_st(n, st):
        """
        n -> status -> [status]
        """
        peg0, peg1, peg2 = st[0], st[1], st[2]
        n0 = list(peg0)
        n0.append(n)
        n1 = list(peg1)
        n1.append(n)
        n2 = list(peg2)
        n2.append(n)
        new0 = (n0, peg1, peg2)
        new1 = (peg0, n1, peg2)
        new2 = (peg0, peg1, n2)
        return [new0, new1, new2]
    def inject_st_all(n, sts):
        xs = []
        for st in sts:
            xs.extend(inject_st(n, st))
        return xs
    next_st = inject_st_all(ng + 1, sts)
    def inject_g(n, pos, g):
        if pos == 0:
            return [((xappend(sta0, n), sta1, sta2),  (xappend(stb0, n), stb1, stb2))
                    for (sta0, sta1, sta2), (stb0, stb1, stb2) in g]
        elif pos == 1:
            return [((sta0, xappend(sta1, n), sta2),  (stb0, xappend(stb1, n), stb2))
                    for (sta0, sta1, sta2), (stb0, stb1, stb2) in g]
        else:
            return [((sta0, sta1, xappend(sta2, n)),  (stb0, stb1, xappend(stb2, n)))
                    for (sta0, sta1, sta2), (stb0, stb1, stb2) in g]
    def inject_g_all(n, g):
        lst = []
        rest = list(range(1, n))
        lst.append((([n], [], rest), ([], [n], rest)))
        if flag == 1:
            lst.append((([n], rest, []), ([], rest, [n]))) # if forbidden directly A to C
        lst.append(((rest, [n], []), (rest, [], [n])))
        g0 = inject_g(n, 0, g)
        g1 = inject_g(n, 1, g)
        g2 = inject_g(n, 2, g)
        ng = []
        ng.extend(lst)
        ng.extend(g0)
        ng.extend(g1)
        ng.extend(g2)
        return ng
    next_g = inject_g_all(ng + 1, g)
    return (next_ng, next_st, next_g)

def showG(g):
    ng, sts, g = g[0], g[1], g[2]
    print(ng)
    for st in sts:
        print(st)
    for a, b in g:
        print(a, " -- ", b)
            
g0 = G0()
showG(g0)

def iterG(g0, flag=1):
    g1 = nextg(g0, flag=flag)
    #print(g1)
    #showG(g1)
    #renderG(g1)
    g2 = nextg(g1, flag=flag)
    #showG(g2)
    #renderG(g2)
    #renderG_file(g2, "genhano2.dot")
    # G2 = buildNx(g2)
    g3 = nextg(g2, flag=flag)
    # showG(g3)
    #renderG(g3)
#renderG_file(g3, "genhano3.dot")
#G3 = buildNx(g3)
#renderNx(G3)



    g4 = nextg(g3, flag=flag)
#showG(g4)
#G4 = buildNx(g4)
#renderNx(G4)
#renderG_file(g4, "genhano4.dot")

    g5 = nextg(g4, flag=flag)
    g6 = nextg(g5, flag=flag)
    #G5 = buildNx(g5)
    #renderNx(G5)
    return [buildNx(g) for g in [g1, g2, g3, g4, g5, g6]]

    #renderG_file(g5, "genhano5.dot")

Gs = iterG(g0)
# renderNx(G5)
Gs0 = iterG(g0, flag=0)

cnt=0
for g, g0 in zip(Gs, Gs0):
    cnt += 1
    pos = nx.kamada_kawai_layout(g)
    plt.figure(figsize=(10, 8))
    node_size = 64 if cnt <= 5 else 16
    nx.draw(g, pos, node_size=node_size, width=2) # !!! print G50, but with G5's layout
    piles = list(range(1, cnt + 1))
    print(piles)
    nodes_to_show = [(piles, [], []), ([], piles, []), ([], [], piles)]
    #nodes_to_show = [renderSt(st) for st in nodes_to_show]
    nx.draw_networkx_labels(g, pos, {renderSt(n): labelSt(n) for n in nodes_to_show})
    plt.savefig("nx%d.png" % (cnt))
    plt.figure(figsize=(10, 8))
    nx.draw(g0, pos, node_size=node_size, width=2)
    nx.draw_networkx_labels(g, pos, {renderSt(n): labelSt(n) for n in nodes_to_show})
    plt.savefig("nx_restricted_%d.png" % (cnt))


def plot_with_different_layouts(G):
    layouts = {
            #'spring': nx.spring_layout,
        'kamada_kawai': nx.kamada_kawai_layout,
        # 'circular': nx.circular_layout,
        # 'random': nx.random_layout,
        # 'shell': nx.shell_layout,
        'spectral': nx.spectral_layout,
        # 'spiral': nx.spiral_layout,
        # 'multipartite': nx.multipartite_layout
    }

    fig, axes = plt.subplots(1, 2, figsize=(20, 10))
    axes = axes.flatten()

    for ax, (name, layout) in zip(axes, layouts.items()):
        print(name)
        pos = layout(G)
        ax.set_title(name)
        nx.draw(G, pos, ax=ax) #, with_labels=True)

    plt.tight_layout()
    plt.show()
#plot_with_different_layouts(G4)
