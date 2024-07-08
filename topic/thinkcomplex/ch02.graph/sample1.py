import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

G = nx.Graph()

positions = dict(Albany=(-74, 43), Boston=(-71, 42), NYC=(-74, 41), Philly=(-75, 40))

G.add_nodes_from(positions)

driver_times = {('Albany', 'Boston'): 3,
                ('Albany', 'NYC'): 4,
                ('Boston', 'NYC'): 4,
                ('NYC', 'Philly'): 2}
G.add_edges_from(driver_times)

print(list(G.nodes()))
print(list(G.edges()))

# nx.draw_circular(G, node_size=2000, with_labels=True)
nx.draw(G, positions, node_shape='s', node_size=1000, with_labels=True)
nx.draw_networkx_edge_labels(G, positions, edge_labels=driver_times)
plt.show()
