import networkx as nx
import numpy as np


if __name__ == '__main__':
# Directed Network
    D = nx.DiGraph()
    #D.add_weighted_edges_from([('A', 'B', 0.5), ('A', 'C', 1)])
    D.add_weighted_edges_from([('1', '2', 25), ('1', '3', 10),('1', '4', 15), ('2', '5', 15), ('3', '5', 10), ('4', '5', 15)])
#D.add_weighted_edges_from([('chi', 'pit', 12), ('car', 'chi', 10),('car', 'tb', 20), ('car', 'no', 3), ('tb', 'chi', 3), ('tb', 'car', 10), ('no', 'car', 3), ('no', 'tb', 14)])
#D.add_weighted_edges_from([('1', '3', 5), ('2', '3', 60),('2', '4', 70), ('2', '5', 200), ('3', '6', 100), ('4', '6', 100), ('4', '7', 250), ('5', '7', 250), ('5', '8', 50)])

#D.add_weighted_edges_from([('1', '2', 25), ('1', '3', 10),('1', '4', 15)])

    #D.add_weighted_edges_from([('1', '2', 1), ('1', '3', 1),('3', '1', 1), ('3', '2', 1), ('3', '5', 1), ('4', '5', 1), ('5', '4', 1), ('4', '6', 1), ('6', '4', 1),('5', '6', 1)])
    #D.add_weighted_edges_from([('0', '1', 150), ('3', '1', 0.5), ('2', '1', 0.5), ('4', '1', 910), ('5', '1', 60), ('7', '1', 600), ('9', '1', 32), ('5', '6', 40), ('7', '6', 40),('8', '4', 670), ('8', '10', 250), ('8', '9', 173), ('8', '11', 500)])
    G = D.to_undirected()

#D.add_weighted_edges_from([('1', '2', 25), ('1', '3', 10),('1', '4', 15)])
# Default max number of iterations failed to converge for me
#pr = nx.pagerank(G, max_iter=200)
    pr = nx.pagerank(G, max_iter=200)

    for key, value in sorted([(value,key) for (key,value) in pr.items()]):
        print ("%s: %s" % (key, value))
