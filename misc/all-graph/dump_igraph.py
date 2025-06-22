#!/usr/bin/env python3

import os
import json
import igraph as ig


def dump_igraph(graph: dict[str, dict]) -> ig.Graph:
    index_map = {x: i for i, x in enumerate(graph)}
    g = ig.Graph(len(graph))
    for index, (layout, info) in enumerate(graph.items()):
        g.vs[index]['code'] = layout
        g.vs[index]['step'] = info['step']
        g.add_edges([(index, index_map[x]) for x in info['next']])
    return g


def convert_ig(file: str, output: str) -> None:
    raw = json.loads(open(file).read())
    dump_igraph(raw['graph']).write_pickle(output)


if __name__ == '__main__':
    for name in sorted(os.listdir('output-json')):
        name = name.removesuffix('.json')
        print(name)
        convert_ig(f'output-json/{name}.json', f'output-ig/{name}.pickle')
