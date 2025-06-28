#!/usr/bin/env python3

import os
import igraph as ig
import multiprocessing


def split_layer(graph: ig.Graph, step_a: int, step_b: int) -> tuple[list[set[int]], list[set[int]]]:

    def extend_from(node: ig.Vertex) -> tuple[set[ig.Vertex], set[ig.Vertex]]:
        assert node['step'] == step_a

        scan_a2b = True
        union_a, union_b = set(), set()
        curr_set, next_set = set([node]), set()

        while curr_set:
            for layout in curr_set:
                for neigh in layout.neighbors():
                    if scan_a2b and neigh['step'] == step_b and neigh not in union_b:
                        next_set.add(neigh)
                    elif not scan_a2b and neigh['step'] == step_a and neigh not in union_a:
                        next_set.add(neigh)

            union_a.update(curr_set if scan_a2b else next_set)
            union_b.update(next_set if scan_a2b else curr_set)
            scan_a2b = not scan_a2b
            curr_set = next_set
            next_set = set()

        return union_a, union_b

    assert step_a + 1 == step_b

    layer_a = set(x for x in graph.vs if x['step'] == step_a)
    layer_b = set(x for x in graph.vs if x['step'] == step_b)
    layer_num_a, layer_num_b = len(layer_a), len(layer_b)
    assert layer_num_a > 0 and layer_num_b > 0

    data_a: list[set[int]] = []
    data_b: list[set[int]] = []
    special_set = set()
    while layer_a:
        union_a, union_b = extend_from(layer_a.pop())
        if len(union_b) == 0:
            assert len(union_a) == 1
            special_set.update(union_a)
            continue
        layer_a -= union_a
        layer_b -= union_b
        data_a.append(set(x.index for x in union_a))
        data_b.append(set(x.index for x in union_b))
    data_a.append(set(x.index for x in special_set))

    assert len(layer_a) == 0 and len(layer_b) == 0
    assert sum(len(x) for x in data_a) == layer_num_a
    assert sum(len(x) for x in data_b) == layer_num_b
    return data_a, [x for x in data_b if x]


def build_multi_set(unions_a: list[set[int]], unions_b: list[set[int]]) -> list[set[int]]:
    assert set(y for x in unions_a for y in x) == set(y for x in unions_b for y in x)

    release = []
    for curr in unions_a:
        for other in unions_b:
            mid = curr.intersection(other)
            if mid:
                release.append(mid)
                curr -= mid
                other -= mid
        assert len(curr) == 0

    assert set(len(x) for x in unions_a) == {0}
    assert set(len(x) for x in unions_b) == {0}
    return release


def do_split(g: ig.Graph) -> ig.Graph:
    max_step = max(x['step'] for x in g.vs)

    layer_data = [[] for _ in range(max_step + 1)]
    layer_data[0].append([set(x.index for x in g.vs if x['step'] == 0)])
    for step in range(0, max_step):
        data_a, data_b = split_layer(g, step, step + 1)
        layer_data[step].append(data_a)
        layer_data[step + 1].append(data_b)
    layer_data[max_step].append([set(x.index for x in g.vs if x['step'] == max_step)])

    assert len(layer_data) == max_step + 1
    assert set(len(x) for x in layer_data) == {2}

    unions = {}
    for step in range(0, max_step + 1):
        layer_unions = build_multi_set(layer_data[step][0], layer_data[step][1])
        for union in layer_unions:
            assert len(set(g.vs[x]['step'] for x in union)) == 1
            codes = [g.vs[x]['code'] for x in union]
            unions[min(codes)] = union

    assert sorted(y for x in unions.values() for y in x) == list(range(g.vcount()))

    combine_info = [-1 for _ in range(g.vcount())]
    for index, key in enumerate(sorted(unions)):
        for x in unions[key]:
            combine_info[x] = index

    assert len(combine_info) == g.vcount()
    assert set(combine_info) == set(range(len(unions)))

    g.contract_vertices(combine_info, combine_attrs={'step': 'first', 'code': list})
    assert set(x.is_loop() for x in g.es) == {False}
    g.simplify(multiple=True)
    return g


def do_combine(input: str, output: str) -> None:
    print(f'Start combining: {input}')
    g = do_split(ig.Graph.Read_Pickle(input))
    g.write_pickle(output)

    g_mod = g.copy()
    for x in g_mod.vs:
        x['code'] = '+'.join(x['code'])
    g_mod = do_split(g_mod)

    assert g.vcount() == g_mod.vcount()
    assert g.ecount() == g_mod.ecount()
    for index in range(g.vcount()):
        assert len(g_mod.vs[index]['code']) == 1
        assert g.vs[index]['step'] == g_mod.vs[index]['step']
        assert '+'.join(g.vs[index]['code']) == g_mod.vs[index]['code'][0]
    assert g.isomorphic(g_mod)


def combine_all(ig_dir: str, output_dir: str) -> None:
    pool = multiprocessing.Pool()
    for name in sorted(os.listdir(ig_dir)):
        pool.apply_async(do_combine, args=(f'{ig_dir}/{name}', f'{output_dir}/{name}'))
    pool.close()
    pool.join()


if __name__ == "__main__":
    os.makedirs('output-combine', exist_ok=True)
    combine_all('output-ig', 'output-combine')
