#!/usr/bin/env python3

import json
from klotski import Group, Layout, FastCal


def layout_to_str(layout: Layout) -> str:
    code = str(layout)
    assert len(code) == 9 and code.endswith('00')
    return code[:7]


def build_graph(group: Group, targets: set[Layout], ignores: set[Layout]) -> dict[Layout, dict]:
    assert targets & ignores == set()

    steps = {x: [] for x in list(group.cases())}
    assert len(steps) == group.size

    for target in targets:
        fc = FastCal(target)
        fc.build_all()
        layers = fc.exports()
        assert sum(len(x) for x in layers) == group.size

        for step, layouts in enumerate(layers):
            [steps[x].append((target, step)) for x in layouts]

    graph = {}
    for layout in steps:
        assert len(steps[layout]) == len(targets)
        step = min(x for _, x in steps[layout])
        pivots = set(x for x, y in steps[layout] if y == step)
        graph[layout] = {'step': step, 'pivots': pivots, 'next': set()}

    for layout, info in graph.items():
        for x in layout.next_cases():
            if graph[x]['step'] == info['step'] + 1:
                info['next'].add(x)

    # for ignore in ignores:
    #     assert ignore in graph
    #     for x in graph[ignore]['next']:
    #         assert x in ignores
    #
    #     for layout, info in graph.items():
    #         if ignore in info['next'] and layout not in ignores:
    #             assert layout in targets

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

    # for layout, info in graph.items():
    #     for x in info['next']:
    #         assert x in graph

    assert sorted(graph) == list(graph)
    assert len(set(graph)) == len(graph)
    return graph


def dump_json(group_info: dict, graph: dict[Layout, dict]) -> str:
    data = {}
    for layout, info in graph.items():
        data[layout_to_str(layout)] = {
            'step': info['step'],
            'next': sorted(layout_to_str(x) for x in info['next']),
            'pivots': sorted(layout_to_str(x) for x in info['pivots']),
        }
    return json.dumps({**group_info, 'graph': data}, separators=(',', ':'))


def load_and_dump(info: dict, path_prefix: str) -> None:
    targets = sorted(Layout(x) for x in info['solutions']['valid'])
    ignores = sorted(Layout(x) for x in info['solutions']['invalid'])

    target_map = {Layout(v): k for k, v in info['targets'].items()}
    assert len(target_map) == len(info['targets'])

    group = targets[0].group
    graph = build_graph(group, set(targets), set(ignores))
    group_info = {
        'size': group.size,
        'targets': {layout_to_str(x): target_map[x] for x in targets},
        'ignores': [layout_to_str(x) for x in ignores],
    }
    with open(f'{path_prefix}.json', 'w') as fp:
        fp.write(dump_json(group_info, graph))


if __name__ == '__main__':
    raw = json.loads(open('data.json').read())

    for name, info in raw.items():
        print(name)
        load_and_dump(info, f'./output-json/{name}')
