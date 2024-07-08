import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

G = nx.DiGraph()

ns = ['Alice', 'Bob', 'Chuck']
for n in ns:
    G.add_node(n)

G.add_edge('Alice', 'Bob')
G.add_edge('Alice', 'Chuck')

print(list(G.nodes()))
print(list(G.edges()))

# nx.draw_circular(G, /*node_color=COLORS[0],*/ node_size=2000, with_labels=True)
nx.draw_circular(G, node_size=2000, with_labels=True)
plt.show()
