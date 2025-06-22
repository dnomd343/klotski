#!/usr/bin/env python3

import os
import json
import igraph as ig


def dump_graph(graph: dict[str, dict]) -> ig.Graph:
    index_map = {x: i for i, x in enumerate(graph)}
    g = ig.Graph(len(graph))
    for index, (layout, info) in enumerate(graph.items()):
        g.vs[index]['code'] = layout
        g.vs[index]['step'] = info['step']
        g.add_edges([(index, index_map[x]) for x in info['next']])
    return g


def dump_sub_graph(graph: dict[str, dict], target: str) -> ig.Graph:
    cases = [x for x, info in graph.items() if target in info['pivots']]
    assert sorted(cases) == cases

    index_map = {x: i for i, x in enumerate(cases)}
    g = ig.Graph(len(cases))

    for index, layout in enumerate(cases):
        info = graph[layout]
        g.vs[index]['code'] = layout
        g.vs[index]['step'] = info['step']

        for x in info['next']:
            assert x in index_map
            g.add_edge(index, index_map[x])

    return g


def convert_ig(file: str, output: str) -> None:
    raw = json.loads(open(file).read())
    g_main = dump_graph(raw['graph'])
    g_main.write_pickle(f'{output}.pkl')

    for layout, sub_tag in raw['targets'].items():
        print(layout, sub_tag)
        g = dump_sub_graph(raw['graph'], layout)
        g.write_pickle(f'{output}-{sub_tag}_{layout}.pkl')


if __name__ == '__main__':
    for name in sorted(os.listdir('output-json')):
        name = name.removesuffix('.json')
        print(name)
        convert_ig(f'output-json/{name}.json', f'output-ig/{name}')
