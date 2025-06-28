#!/usr/bin/env python3

import os
import igraph as ig


def load_legacy(file: str) -> ig.Graph:
    g = ig.Graph.Read_Pickle(file)
    for node in g.vs:
        node['code'] = sorted(node['code'])
    return g


def load_modern(file: str) -> ig.Graph:
    g = ig.Graph.Read_Pickle(file)
    for node in g.vs:
        assert sorted(node['code']) == sorted(node['code'])
    return g


def compare(g1: ig.Graph, g2: ig.Graph) -> None:
    assert g1.vcount() == g2.vcount()
    assert g1.ecount() == g2.ecount()
    assert g1.isomorphic(g2)

    assert {len(x.attributes()) for x in g1.es} == {0}
    assert {len(x.attributes()) for x in g2.es} == {0}

    data_a = {min(x['code']): x.attributes() for x in g1.vs}
    data_b = {min(x['code']): x.attributes() for x in g2.vs}
    assert data_a == data_b


if __name__ == '__main__':
    for name in sorted(os.listdir('output-combine')):
        if '_' not in name:
            continue
        g1 = load_legacy(f'combined/{name.split('_')[1]}')
        g2 = load_modern(f'output-combine/{name}')
        compare(g1, g2)
