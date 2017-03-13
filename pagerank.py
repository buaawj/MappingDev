import networkx as nx
import numpy as np

# Directed Network
D = nx.DiGraph()
#D.add_weighted_edges_from([('A', 'B', 0.5), ('A', 'C', 1)])
#D.add_weighted_edges_from([('1', '2', 25), ('1', '3', 10),('1', '4', 15), ('2', '5', 15), ('3', '5', 10), ('4', '5', 15)])
#D.add_weighted_edges_from([('chi', 'pit', 12), ('car', 'chi', 10),('car', 'tb', 20), ('car', 'no', 3), ('tb', 'chi', 3), ('tb', 'car', 10), ('no', 'car', 3), ('no', 'tb', 14)])
D.add_weighted_edges_from([('1', '3', 5), ('2', '3', 60),('2', '4', 70), ('2', '5', 200), ('3', '6', 100), ('4', '6', 100), ('4', '7', 250), ('5', '7', 250), ('5', '8', 50)])

# Convert to undirected
#G = D.to_undirected()

# Default max number of iterations failed to converge for me
pr = nx.pagerank(D, max_iter=200)
print (pr)
