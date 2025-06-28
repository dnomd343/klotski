#!/usr/bin/env python3

import igraph as ig


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
    while layer_a:
        # TODO: maybe we should combine all union_a when union_b is empty
        union_a, union_b = extend_from(layer_a.pop())
        layer_a -= union_a
        layer_b -= union_b
        data_a.append(set(x.index for x in union_a))
        data_b.append(set(x.index for x in union_b))

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


def do_split(file: str) -> ig.Graph:
    g = ig.Graph.Read_Pickle(file)
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


if __name__ == "__main__":
    g_release = do_split('output-ig/1-00M-000Y_DAA7F30.pkl')
    g_release.write_pickle('1-00M-000Y_DAA7F30-combined.pkl')
