#!/usr/bin/env python3

import os
import igraph as ig


def load_legacy(file: str) -> ig.Graph:
    g = ig.Graph.Read_Pickle(file)
    for node in g.vs:
        assert sorted(node['code']) == node['code']
        node['codes'] = node['code']
    del g.vs['code']
    return g


def load_modern(file: str) -> ig.Graph:
    g = ig.Graph.Read_Pickle(file)
    assert [int(x.removeprefix('U')) for x in g.vs['id']] == list(range(g.vcount()))
    for node in g.vs:
        assert sorted(node['codes']) == node['codes']
    del g.vs['id']
    return g


def compare(g1: ig.Graph, g2: ig.Graph) -> None:
    assert g1.vcount() == g2.vcount()
    assert g1.ecount() == g2.ecount()
    assert g1.isomorphic(g2)

    assert {len(x.attributes()) for x in g1.es} == {0}
    assert {len(x.attributes()) for x in g2.es} == {0}

    data_a = {min(x['codes']): x.attributes() for x in g1.vs}
    data_b = {min(x['codes']): x.attributes() for x in g2.vs}
    assert data_a == data_b


if __name__ == '__main__':
    for name in sorted(os.listdir('output-combine-raw')):
        g1 = load_legacy(f'output-combine-raw/{name}')
        g2 = load_modern(f'output-combine/{name}')
        compare(g1, g2)
