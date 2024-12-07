#!/usr/bin/env python3

import igraph as ig
from itertools import takewhile


def load_graph(pkl_file: str) -> ig.Graph:
    graph = ig.Graph.Read_Pickle(pkl_file)
    for i in range(graph.vcount()):
        graph.vs[i]['codes'] = ['|'.join(graph.vs[i]['codes'])]
    print(graph.summary())
    return graph


def find_fold_points(step_list: list[int], degree_list: list[int]) -> list[tuple[int, int]]:
    size = len(step_list)
    assert size == len(degree_list) and size > 0 and size % 2 == 0
    half_size = size // 2

    def fix_index(raw_id: int) -> int:
        if raw_id < 0:
            return raw_id + size
        elif raw_id >= size:
            return raw_id - size
        return raw_id

    fold_info = []
    for index in range(size):
        right_ids = [fix_index(x) for x in range(index + 1, index + half_size)]
        left_ids = [fix_index(x) for x in range(index - 1, index - half_size, -1)]
        assert len(left_ids) == len(right_ids)

        steps = ((step_list[a], step_list[b]) for a, b in zip(left_ids, right_ids))
        fold_len = len(list(takewhile(lambda x: x[0] == x[1], steps)))
        fold_ids = list(zip(left_ids[:fold_len], right_ids[:fold_len]))
        fold_degree = sum(degree_list[a] + degree_list[b] for a, b in fold_ids)

        assert fold_len < half_size
        assert fold_degree >= 4 * fold_len
        assert len(fold_ids) == fold_len
        fold_info.append({
            'index': index,
            'fold_len': fold_len,
            'fold_degree': fold_degree,
            'fold_ids': fold_ids,
        })
        # print(f'{index}: {fold_degree} {fold_ids} ({fold_len})')

    # need larger `fold_len` and smaller `fold_degree`
    fold_info.sort(key=lambda x: (-x['fold_len'], x['fold_degree']))  # maybe we need min_common_code
    return fold_info[0]['fold_ids']


def combine_points(graph: ig.Graph, pairs: list[tuple[int, int]]) -> None:
    graph.vs['id'] = range(graph.vcount())
    for id_a, id_b in pairs:
        points_a = [x for x in graph.vs if x['id'] == id_a]
        points_b = [x for x in graph.vs if x['id'] == id_b]
        assert len(points_a) == 1 and len(points_b) == 1

        point_a, point_b = points_a[0], points_b[0]
        assert point_a['step'] == point_b['step']

        neighs_a = point_a.neighbors()
        neighs_b = point_b.neighbors()
        neighs = sorted(set(neighs_a) | set(neighs_b))
        assert len(neighs_a) >= 2 and len(neighs_b) >= 2

        point_new = graph.add_vertex()
        point_new['step'] = point_a['step']
        point_new['codes'] = point_a['codes'] + point_b['codes']
        # print(point_new)
        graph.add_edges([(neigh.index, point_new.index) for neigh in neighs])
        graph.delete_vertices([point_a.index, point_b.index])


def fold_circle(graph: ig.Graph) -> None:
    while True:
        print(g.summary())
        if g.is_tree():
            break
        circle = g.girth(return_shortest_circle=True)
        circle = [graph.vs[x] for x in circle]
        fold_pairs = find_fold_points([x['step'] for x in circle], [x.degree() for x in circle])
        fold_pairs = [(circle[a].index, circle[b].index) for a, b in fold_pairs]
        print('fold:', fold_pairs)
        combine_points(graph, fold_pairs)


if __name__ == '__main__':
    # val = find_fold_point([53, 52, 51, 52, 53, 54, 55, 56, 57, 56, 55, 54], [2, 2, 3, 2, 2, 2, 2, 2, 3, 2, 2, 2])
    # val = find_fold_point([65, 66, 67, 66, 65, 64, 65, 66, 67, 66, 65, 64, 63, 64], [2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 3, 2])
    # val = find_fold_point([53, 54, 55, 54, 53, 54, 53, 52, 51, 52], [2, 2, 2, 2, 3, 2, 2, 2, 3, 2])
    # print(val)

    g = load_graph('main_combined.pkl')
    fold_circle(g)
    g.write_pickle('main_circle.pkl')
    # g.write_graphml('main_circle.graphml')

    # g = load_graph('main_cut.pkl')
    # fold_circle(g)
    # g.write_pickle('main_cut_circle.pkl')
    # g.write_graphml('main_cut_circle.graphml')
