import networkx as nx
import numpy as np

def pagerank(graph, damping_factor=0.85, max_iterations=100, min_delta=0.00001):
    """
    Compute and return the PageRank in an directed graph.

    @type  graph: digraph
    @param graph: Digraph.

    @type  damping_factor: number
    @param damping_factor: PageRank dumping factor.

    @type  max_iterations: number
    @param max_iterations: Maximum number of iterations.

    @type  min_delta: number
    @param min_delta: Smallest variation required to have a new iteration.

    @rtype:  Dict
    @return: Dict containing all the nodes PageRank.
    """

    nodes = graph.nodes()
    graph_size = len(nodes)
    if graph_size == 0:
        return {}
    min_value = (1.0-damping_factor)/graph_size #value for nodes without inbound links

    # itialize the page rank dict with 1/N for all nodes
    pagerank = dict.fromkeys(nodes, 1.0/graph_size)
    print(pagerank)
    for i in range(max_iterations):
        diff = 0 #total difference compared to last iteraction
        # computes each node PageRank based on inbound links
        for node in nodes:
            rank = min_value
            for referring_page in graph.predecessors(node):
                rank += damping_factor * pagerank[referring_page] / len(graph.successors(referring_page))

            diff += abs(pagerank[node] - rank)
            pagerank[node] = rank

        #stop if PageRank has converged
        if diff < min_delta:
            break

    return pagerank


if __name__ == '__main__':
    dg = nx.DiGraph()
    #dg.add_weighted_edges_from([('A', 'B', 1)])
    #dg.add_weighted_edges_from([('A', 'C', 1)])
    #dg.add_weighted_edges_from([('A', 'D', 1)])
    #dg.add_weighted_edges_from([('B', 'D', 1)])
    #dg.add_weighted_edges_from([('C', 'E', 1)])
    #dg.add_weighted_edges_from([('D', 'E', 1)])
    #dg.add_weighted_edges_from([('B', 'E', 1)])
    #dg.add_weighted_edges_from([('E', 'A', 1)])
    dg.add_weighted_edges_from([('A', 'B', 0.5), ('A', 'C', 1)])

    pr = pagerank(dg)

    print("The final page rank is:\n", pr)
