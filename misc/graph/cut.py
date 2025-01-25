#!/usr/bin/env python3

import igraph as ig


def split_max_graph(g: ig.Graph, cut_point: int) -> ig.Graph:
    gg = g.copy()
    edges = [(cut_point, x.index) for x in gg.vs[cut_point].neighbors()]
    gg.delete_edges(edges)
    components = sorted(gg.components(), key=lambda x: -len(x))
    return g.subgraph(components[0] + [cut_point])


def split_subgraph(g: ig.Graph) -> ig.Graph:
    while True:
        points = g.articulation_points()
        if len(points) == 0:
            break
        g = split_max_graph(g, points[0])
        print(g.vcount(), g.ecount())
    return g


if __name__ == '__main__':
    raw = ig.Graph.Read_Pickle('main_combined.pkl')
    split_subgraph(raw).write_pickle('main_cut.pkl')
    # split_subgraph(raw).write_graphml('main_cut.graphml')
