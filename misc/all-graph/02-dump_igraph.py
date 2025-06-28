#!/usr/bin/env python3

import os
import json
import igraph as ig
import multiprocessing


def save_graph(graph: ig.Graph, path: str) -> None:
    edges = graph.get_edgelist()
    for edge in edges:
        assert edge[0] < edge[1]

    graph.delete_edges()
    graph.add_edges(sorted(edges))
    graph.write_pickle(path)


def dump_graph(data: dict[str, dict]) -> ig.Graph:
    graph = ig.Graph(len(data))
    index_map = {x: i for i, x in enumerate(data)}

    edges = []
    for index, (layout, info) in enumerate(data.items()):
        graph.vs[index]['code'] = layout
        graph.vs[index]['step'] = info['step']
        edges.extend((index, index_map[x]) for x in info['next'])

    graph.add_edges(edges)
    return graph


def dump_sub_graph(data: dict[str, dict], target: str) -> ig.Graph:
    cases = [x for x, info in data.items() if target in info['pivots']]
    assert sorted(cases) == cases

    graph = ig.Graph(len(cases))
    index_map = {x: i for i, x in enumerate(cases)}

    edges = []
    for index, layout in enumerate(cases):
        info = data[layout]
        graph.vs[index]['code'] = layout
        graph.vs[index]['step'] = data[layout]['step']
        edges.extend((index, index_map[x]) for x in info['next'])

    graph.add_edges(edges)
    return graph


def dump_sub_graph_pro(g_all: ig.Graph, target: str) -> ig.Graph:  # without pivot needed
    cases = set()
    queue = [g_all.vs.select(code=target)[0].index]
    while queue:
        layout = queue.pop(0)
        if layout in cases:
            continue
        cases.add(layout)

        step = g_all.vs[layout]['step']
        for neigh in g_all.neighbors(layout):
            if neigh not in cases and g_all.vs[neigh]['step'] == step + 1:
                queue.append(neigh)

    return g_all.induced_subgraph(sorted(cases))


def convert_and_dump(file: str, output_prefix: str) -> None:
    print(f'Start coverting: {file}')

    raw = json.loads(open(file).read())
    graph = dump_graph(raw['graph'])
    save_graph(graph, f'{output_prefix}.pkl')

    for layout, sub_tag in raw['targets'].items():
        sub_graph = dump_sub_graph(raw['graph'], layout)
        # sub_graph = dump_sub_graph_pro(graph, layout)
        save_graph(sub_graph, f'{output_prefix}-{sub_tag}_{layout}.pkl')


def convert_all(json_dir: str, output_dir: str) -> None:
    pool = multiprocessing.Pool()
    for name in sorted(os.listdir(json_dir)):
        json_file = f'{json_dir}/{name}'
        output_prefix = f'{output_dir}/{name.removesuffix('.json')}'
        pool.apply_async(convert_and_dump, args=(json_file, output_prefix))
    pool.close()
    pool.join()


if __name__ == '__main__':
    os.makedirs('output-ig', exist_ok=True)
    convert_all('output-json', 'output-ig')
