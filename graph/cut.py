#!/usr/bin/env python3

import igraph as ig


def split_max_graph(g: ig.Graph, cut_point: int):
    gg = g.copy()
    edges = [(cut_point, x.index) for x in gg.vs[cut_point].neighbors()]
    gg.delete_edges(edges)
    components = sorted(gg.components(), key=lambda x: -len(x))
    return g.subgraph(components[0] + [cut_point])


def split_subgraph(g: ig.Graph):

    # def demo():
    #     sizes = set()
    #     for cut_point in g.articulation_points():
    #         gg: ig.Graph = g.copy()
    #         gg.delete_vertices(cut_point)
    #         subs = gg.decompose()
    #         sizes.update([x.vcount() for x in subs])
    #     print(sorted(sizes))

    # demo()

    for _ in range(1034):
        print(g.vcount(), g.ecount())

        points = g.articulation_points()
        assert len(points) > 0
        g = split_max_graph(g, points[0])

    # ig.plot(g, 'demo.png', bbox=(2000, 2000))
    print(g.vcount(), g.ecount())
    # g.write_graphml('main_cir.graphml')
    g.write_pickle('main_cir.pkl')


if __name__ == '__main__':
    raw = ig.Graph.Read_Pickle('main_combined.pkl')
    # print(raw.vcount())
    split_subgraph(raw)
