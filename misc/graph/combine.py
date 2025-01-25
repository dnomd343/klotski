#!/usr/bin/env python3

import igraph as ig


def split_neighbor_layer(g: ig.Graph, n: int) -> tuple[list[set[ig.Vertex]], list[set[ig.Vertex]]]:
    layer_a: set[ig.Vertex] = set(g.vs.select(step=n))
    layer_b: set[ig.Vertex] = set(g.vs.select(step=n+1))

    layer_a_data: list[set[ig.Vertex]] = []
    layer_b_data: list[set[ig.Vertex]] = []

    select_next = lambda p: set(x for x in p.neighbors() if x['step'] == n + 1)
    select_last = lambda p: set(x for x in p.neighbors() if x['step'] == n)

    while len(layer_a) != 0:
        side_a: set[ig.Vertex] = set()
        side_b: set[ig.Vertex] = set()

        point = layer_a.pop()
        side_a.add(point)

        while True:
            nexts = set()
            for x in side_a:
                nexts.update(select_next(x))
            if nexts == side_b:
                break
            side_b.update(nexts)
            for x in nexts:
                if x in layer_b:
                    layer_b.remove(x)

            lasts = set()
            for x in side_b:
                lasts.update(select_last(x))
            if lasts == side_a:
                break
            side_a.update(lasts)
            for x in lasts:
                if x in layer_a:
                    layer_a.remove(x)

        layer_a_data.append(side_a)
        layer_b_data.append(side_b)

    assert len(layer_b) == 0
    return layer_a_data, layer_b_data


def combine_split_result(data_a: list[set[ig.Vertex]], data_b: list[set[ig.Vertex]]) -> list[set[ig.Vertex]]:
    result = []

    while True:
        data_a = list(filter(None, data_a))
        data_b = list(filter(None, data_b))
        data_a = sorted(data_a, key=lambda x: len(x))
        data_b = sorted(data_b, key=lambda x: len(x))

        if len(data_a) == 0:
            assert len(data_b) == 0
            break

        # print(len(data_a))
        # for x in data_a:
        #     print([y.attributes() for y in x])
        # print(len(data_b))
        # for x in data_b:
        #     print([y.attributes() for y in x])
        # print('-' * 64)

        if len(data_a[0]) <= len(data_b[0]):
            union = data_a[0]
            peer_unions = data_b
        else:
            union = data_b[0]
            peer_unions = data_a

        for peer_union in peer_unions:
            mid = union & peer_union
            if len(mid) == 0:
                continue
            result.append(mid)
            for x in mid:
                union.remove(x)
                peer_union.remove(x)

    return result


def split_layers(g: ig.Graph) -> list[list[set[ig.Vertex]]]:
    assert min(g.vs['step']) == 0
    layer_num = max(g.vs['step']) + 1
    print(f'layer_num: {layer_num}')

    layers = [{'up': [], 'down': []} for x in range(layer_num)]
    layers[0]['up'] = [set(g.vs.select(step=0))]
    layers[-1]['down'] = [set(g.vs.select(step=layer_num-1))]

    for layer_num in range(layer_num - 1):
        data_a, data_b = split_neighbor_layer(g, layer_num)
        layers[layer_num]['down'] = data_a
        layers[layer_num + 1]['up'] = list(filter(None, data_b))

    for layer in layers:
        assert set() not in layer['up']
        assert set() not in layer['down']
        up = set()
        [up.update(x) for x in layer['up']]
        down = set()
        [down.update(x) for x in layer['down']]
        assert up == down

    result = []
    for layer in layers:
        result.append(combine_split_result(layer['up'], layer['down']))

    for layer_num, layer in enumerate(result):
        assert set() not in layer
        layer_nodes = set()
        [layer_nodes.update(x) for x in layer]
        assert layer_nodes == set(g.vs.select(step=layer_num))
    return result


def export_new_graph(g: ig.Graph, split_data: list[list[set[ig.Vertex]]]) -> ig.Graph:
    ng = ig.Graph(sum([len(x) for x in split_data]))

    g_index = 0
    index_map = {}
    for layer_index, unions in enumerate(split_data):
        for union_index, nodes in enumerate(unions):
            index_map[(layer_index, union_index)] = g_index
            ng.vs[g_index]['step'] = layer_index
            ng.vs[g_index]['codes'] = [x['code'] for x in nodes]
            g_index += 1

    for layer_index in range(len(split_data)-1):
        curr_layer = split_data[layer_index]
        next_layer = split_data[layer_index+1]

        def union_neighbors(curr_union_index: int) -> list[int]:
            next_union_indexes = set()
            for node in curr_layer[curr_union_index]:
                next_nodes = [x for x in node.neighbors() if x['step'] == layer_index+1]
                for next_node in next_nodes:
                    for next_union_index in range(len(next_layer)):
                        if next_node in next_layer[next_union_index]:
                            next_union_indexes.add(next_union_index)
            return sorted(next_union_indexes)

        for union_index in range(len(curr_layer)):
            union_a = index_map[(layer_index, union_index)]
            edges = [(union_a, index_map[(layer_index+1, x)]) for x in union_neighbors(union_index)]
            ng.add_edges(edges)

    return ng


if __name__ == '__main__':
    raw = ig.Graph.Read_Pickle('data/DAA7F30.pkl')
    # raw = ig.Graph.Read_Pickle('data/DBAB4CC.pkl')
    # raw = ig.Graph.Read_Pickle('main_combined.pkl')
    print(raw.summary())
    gg = export_new_graph(raw, split_layers(raw))
    print(gg.summary())
    # print(gg.isomorphic(raw))

    gg.write_pickle('main_combined.pkl')
    # gg.write_graphml('main_combined.graphml')
