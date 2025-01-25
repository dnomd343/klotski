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


def fold_circle(graph: ig.Graph, limit: int | None) -> None:
    while True:
        # print(graph.summary())
        if graph.is_tree():
            break
        ring = [graph.vs[x] for x in graph.girth(return_shortest_circle=True)]
        if limit is not None:
            if len(ring) > limit:
                break

        fold_pairs = find_fold_points([x['step'] for x in ring], [x.degree() for x in ring])
        fold_pairs = [(ring[a].index, ring[b].index) for a, b in fold_pairs]
        print(f'fold: {fold_pairs}')
        combine_points(graph, fold_pairs)
    del graph.vs['id']


def export_graph(graph: ig.Graph, pkl_file: str) -> None:
    for i in range(graph.vcount()):
        graph.vs[i]['num'] = sum(len(x.split('|')) for x in graph.vs[i]['codes'])
        graph.vs[i]['codes'] = '-'.join(graph.vs[i]['codes'])
    graph.write_pickle(pkl_file)


if __name__ == '__main__':
    raw = load_graph('main_combined.pkl')
    fold_circle(raw, None)
    # fold_circle(raw, 32)
    export_graph(raw, 'main_folded.pkl')
