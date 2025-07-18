#!/usr/bin/env python3

import os
import igraph as ig
import multiprocessing

type Union = set[int]


def split_adjacent_layers(graph: ig.Graph, step: int) -> tuple[list[Union], list[Union]]:
    layouts = graph.vs.select(step_in=[step, step + 1])
    mapping = {x['code']: x.index for x in layouts}
    spawn_union = lambda iter: {mapping[x['code']] for x in iter}

    layer_curr, layer_next = [], []
    g_focus = graph.subgraph(layouts)
    if isolated := g_focus.vs.select(_degree=0):
        assert set(isolated['step']) == {step}
        layer_curr = [spawn_union(isolated)]
        g_focus.delete_vertices(isolated)

    for comp in map(g_focus.vs.select, g_focus.connected_components()):
        layer_curr.append(spawn_union(comp.select(step=step)))
        layer_next.append(spawn_union(comp.select(step=step+1)))
    return layer_curr, layer_next


def apply_layer_unions(unions_a: list[Union], unions_b: list[Union]) -> list[Union]:
    layouts = {x for u in unions_a for x in u}
    assert layouts == {x for u in unions_b for x in u}

    unions = []
    for curr_union in unions_a:
        for other_union in unions_b:
            if union := curr_union.intersection(other_union):
                unions.append(union)
                curr_union -= union
                other_union -= union
        assert len(curr_union) == 0

    assert set(len(x) for x in unions_a) == {0}
    assert set(len(x) for x in unions_b) == {0}
    assert layouts == {x for u in unions for x in u}
    return unions


def build_all_unions(graph: ig.Graph) -> list[Union]:
    max_step = max(graph.vs['step'])
    layer_unions = [[set(graph.vs.select(step=0).indices)]]
    for step in range(0, max_step):
        layer_unions.extend(list(split_adjacent_layers(graph, step)))
    layer_unions.append([set(graph.vs.select(step=max_step).indices)])
    assert len(layer_unions) == (max_step + 1) * 2

    all_unions = []
    for idx in range(0, len(layer_unions), 2):
        all_unions.extend(apply_layer_unions(*layer_unions[idx:idx + 2]))
    for unions in all_unions:
        assert len(unions) > 0
        assert len(set(graph.vs[x]['step'] for x in unions)) == 1
    return sorted(all_unions, key=lambda u: min(graph.vs[x]['code'] for x in u))


def combine_graph(graph: ig.Graph) -> ig.Graph:
    unions = build_all_unions(graph)
    union_idx = sorted((x, idx) for idx, u in enumerate(unions) for x in u)

    combine_idx = [x for _, x in union_idx]
    assert len(combine_idx) == graph.vcount()
    assert set(combine_idx) == set(range(len(unions)))

    tag_len = len(str(len(unions) - 1))
    graph.vs['tag'] = [f'U{x:0{tag_len}}' for x in combine_idx]

    graph.contract_vertices(combine_idx, combine_attrs={'tag': 'first', 'step': 'first', 'code': list})
    assert [int(x.removeprefix('U')) for x in graph.vs['tag']] == list(range(len(unions)))
    assert not any(x.is_loop() for x in graph.es)
    graph.simplify(multiple=True)
    return graph


def do_combine(input: str, output: str) -> None:
    print(f'Start combining: {input}')

    g_raw = (graph := combine_graph(ig.Graph.Read_Pickle(input))).copy()
    graph.vs['codes'] = graph.vs['code']
    del graph.vs['code']
    graph.write_pickle(output)  # save combined graph

    g_raw.vs['code'] = g_raw.vs['tag']  # modify as origin format
    g_mod = combine_graph(g_raw.copy())

    assert g_raw.vcount() == g_mod.vcount()
    assert g_raw.ecount() == g_mod.ecount()
    assert all(x['code'] == [x['tag']] for x in g_mod.vs)
    assert g_raw.vs['step'] == g_mod.vs['step']
    assert g_raw.vs['code'] == g_mod.vs['tag']
    assert g_raw.isomorphic(g_mod)


def combine_all(ig_dir: str, output_dir: str) -> None:
    pool = multiprocessing.Pool()
    for name in sorted(os.listdir(ig_dir)):
        pool.apply_async(do_combine, args=(f'{ig_dir}/{name}', f'{output_dir}/{name}'))
    pool.close()
    pool.join()


if __name__ == '__main__':
    os.makedirs('output-combine', exist_ok=True)
    combine_all('output-ig', 'output-combine')
