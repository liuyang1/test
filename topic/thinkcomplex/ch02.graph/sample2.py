import sys
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

def pairs(xs):
    if len(xs) == 0 or len(xs) == 1:
        return []
    lst = [(xs[0], y) for y in xs[1:]]
    return lst + pairs(xs[1:])

def complete_graph(n):
    G = nx.Graph()
    nodes = range(n)
    G.add_nodes_from(nodes)
    G.add_edges_from(pairs(nodes))
    return G

def test5():
    G = complete_graph(7)

    print(list(G.nodes()))
    print(list(G.edges()))

    plt.gca().set_aspect("equal")
    nx.draw_circular(G, node_size=1000, with_labels=True)
    # nx.draw(G, positions, node_shape='s', node_size=1000, with_labels=True)
    # nx.draw_networkx_edge_labels(G, positions, edge_labels=driver_times)
    plt.show()


def reachable_nodes(G, start):
    seen = set()
    stack = [start]
    while stack:
        node = stack.pop()
        if node not in seen:
            seen.add(node)
            stack.extend(G.neighbors(node))
    return seen

def is_connected(G):
    first = next(iter(G)) # get first node in G
    reachable = reachable_nodes(G, first)
    return len(reachable) == len(G)


def random_pairs(nodes, p):
    for e in pairs(nodes):
        if flip(p):
            yield e


def flip(p):
    return np.random.random() < p


def make_random_graph(n, p):
    G = nx.Graph()
    nodes = range(n)
    G.add_nodes_from(nodes)
    G.add_edges_from(random_pairs(nodes, p))
    return G


def test6():
    rg = make_random_graph(10, 0.28)
    nx.draw(rg, with_labels=True)
    plt.show()

# test6()


def prob_connected(n, p, iters=100):
    tf = [is_connected(make_random_graph(n, p)) for i in range(iters)]
    return np.mean(tf) #, np.std(tf)
    # This std is meanlingless, it fixed to {0, 1}
    # std = sqrt(p(1-p))


def sample_prob(n, p, init_iter=100, bulk=10):
    thr, iters, std = 0.0001, init_iter, 1
    while True:
        ps = [prob_connected(n, p, iters=iters) for _ in range(bulk)]
        connected_p, std = np.mean(ps), np.std(ps)
        print(n, p, iters, connected_p, std)
        iters = int(iters * 2)
        if std < thr:
            return connected_p, std


def sample_prob_bulk(n, p, init_iter=100, bulk=10):
    thr, iters, std = 0.0001, init_iter, 1
    ps = []
    while True:
        t = [prob_connected(n, p, iters=iters) for _ in range(bulk)]
        ps.extend(t)
        connected_p, std = np.mean(ps), np.std(ps)
        print(iters, len(ps), connected_p, std)
        bulk *= 2
        if std < thr:
            return connected_p, std

def test7():
    print(sample_prob(10, 0.23026)) # correct answer: is 0.3478 eps=1.1*10^-4
    # print(sample_prob_bulk(10, 0.23)) # correct answer: is 0.3465 eps=5.7*10^-5


# test7()

def sample_y():
    n = 10
    ps = np.logspace(-2.5, 0, 10)
    yp = {p: prob_connected(n, p, iters=1000) for p in ps}

def test8():
    n = 10
    ps = np.logspace(-1, 0, 101)
    ys = [prob_connected(n, p, iters=1000) for p in ps]
    plt.semilogx(ps, ys, "o-")
    plt.xlabel("prob of edges")
    plt.ylabel("prob connected")
    plt.show()

test8()
