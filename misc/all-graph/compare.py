#!/usr/bin/env python3

import os
import igraph as ig


def compare(file_1: str, file_2: str) -> None:
    print(f'{file_1} vs {file_2}')
    g1 = ig.Graph.Read_Pickle(file_1)
    g2 = ig.Graph.Read_Pickle(file_2)

    assert g1.vcount() == g2.vcount()
    assert g1.ecount() == g2.ecount()
    assert g1.isomorphic(g2)

    for edge in g1.es:
        assert edge.attributes() == {}

    for edge in g2.es:
        assert edge.attributes() == {}

    gv1 = {x['code']: x.attributes() for x in g1.vs}
    gv2 = {x['code']: x.attributes() for x in g2.vs}
    assert gv1 == gv2


if __name__ == '__main__':
    assert sorted(os.listdir('legacy-ig')) == sorted(os.listdir('modern-ig'))
    for name in sorted(os.listdir('modern-ig')):
        compare(f'legacy-ig/{name}', f'modern-ig/{name}')
