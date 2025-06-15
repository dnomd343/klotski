#!/usr/bin/env python3

import json
import igraph as ig
from klotski import Group, Layout, FastCal


def layout_to_str(layout: Layout) -> str:
    code = str(layout)
    assert len(code) == 9 and code.endswith('00')
    return code[:7]


def build_graph(group: Group, targets: set[Layout], ignores: set[Layout]) -> dict[Layout, dict]:
    steps = {x: [] for x in list(group.cases())}
    assert len(steps) == group.size

    for target in targets:
        fc = FastCal(target)
        fc.build_all()
        layers = fc.exports()
        assert sum(len(x) for x in layers) == group.size

        for step, layouts in enumerate(layers):
            [steps[x].append(step) for x in layouts]

    graph = {}
    for layout in steps:
        assert len(steps[layout]) == len(targets)
        graph[layout] = {'step': min(steps[layout]), 'next': set()}

    for layout, info in graph.items():
        for x in layout.next_cases():
            if graph[x]['step'] == info['step'] + 1:
                info['next'].add(x)

    # print('valid', graph[Layout('DAFA730')])
    # print('invalid', graph[Layout('D0ABDBC')])
    # print('normal', graph[Layout('1a9bf0c')])

    # print('D0ABDBC', graph[Layout('D0ABDBC')])
    # print('DCA8DBC', graph[Layout('DCA8DBC')])
    # print('DFA81BC', graph[Layout('DFA81BC')])
    # print('DFA21BC', graph[Layout('DFA21BC')])
    # print('DFAA4CC', graph[Layout('DFAA4CC')])

    for ignore in ignores:
        assert ignore in graph
        for x in graph[ignore]['next']:
            assert x in ignores

        for layout, info in graph.items():
            if ignore in info['next'] and layout not in ignores:
                assert layout in targets

    for ignore in ignores:
        del graph[ignore]

    for layout, info in graph.items():
        need_remove = []
        for x in info['next']:
            if x in ignores:
                assert layout in targets
                need_remove.append(x)
        for x in need_remove:
            info['next'].remove(x)

    for layout, info in graph.items():
        for x in info['next']:
            assert x in graph

    return graph


def dump_json(graph: dict[Layout, dict]) -> str:
    data = {}
    for layout, info in graph.items():
        data[layout_to_str(layout)] = {
            'step': info['step'],
            'next': [layout_to_str(x) for x in info['next']]
        }
    return json.dumps(data, separators=(',', ':'))


def dump_igraph(graph: dict[Layout, dict]) -> ig.Graph:
    index_map = {x: i for i, x in enumerate(graph)}
    g = ig.Graph(len(graph))
    for index, (layout, info) in enumerate(graph.items()):
        g.vs[index]['code'] = layout_to_str(layout)
        g.vs[index]['step'] = info['step']
        g.add_edges([(index, index_map[x]) for x in info['next']])
    return g


def load_and_dump(info: dict, path_prefix: str) -> None:
    targets = [Layout(x) for x in info['solutions']['valid']]
    ignores = set(Layout(x) for x in info['solutions']['invalid'])
    graph = build_graph(targets[0].group, set(targets), ignores)

    with open(f'{path_prefix}.json', 'w') as fp:
        fp.write(dump_json(graph))

    ig_graph = dump_igraph(graph)
    ig_graph.write_pickle(f'{path_prefix}.pickle')
    # ig_graph.write_picklez(f'{path_prefix}.picklez')
    ig_graph.write_graphml(f'{path_prefix}.graphml')
    # ig_graph.write_graphmlz(f'{path_prefix}.graphmlz')


if __name__ == '__main__':
    raw = json.loads(open('data.json').read())
    # raw = {'1-00M': raw['1-00M']}

    for name, info in raw.items():
        print(name)
        load_and_dump(info, f'./output/{name}')
