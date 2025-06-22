#!/usr/bin/env python3

import pickle
import igraph as ig

from klotski import FastCal
from klotski import Block, Layout
from klotski import Group, GroupUnion


def is_valid_solution(code: Layout) -> bool:
    if str(code)[0] != 'D':
        return False

    seq = code.dump_seq()
    is_z_valid = seq[9] == Block.SPACE and seq[10] == Block.SPACE
    is_x_valid = seq[12] == Block.SPACE and seq[16] == Block.SPACE
    is_y_valid = seq[15] == Block.SPACE and seq[19] == Block.SPACE
    return is_x_valid or is_y_valid or is_z_valid


def build_step_map(code: str) -> dict[Layout, int]:
    cal = FastCal(Layout(code))
    cal.build_all()
    data = cal.exports()
    result = {}
    for step_num, cases in enumerate(data):
        for case in cases:
            result[case] = step_num
    return result


def build_min_step_scope(group: Group, targets: list[str]) -> dict[Layout, dict[Layout, int]]:
    targets = [Layout(x) for x in targets]
    all_solution = set([x for x in group.cases() if str(x).startswith('D')])
    not_solved = all_solution - set(targets)

    step_data = {x: {} for x in group.cases() if x not in not_solved}
    for target in targets:
        for code, step_num in build_step_map(target).items():
            if code in not_solved:
                continue
            step_data[code][target] = step_num

    min_step_data = {}
    for code, step_info in step_data.items():
        min_step = min([x for x in step_info.values()])
        min_step_cases = [x for x, y in step_info.items() if y == min_step]
        min_step_data[code] = (min_step, min_step_cases)

    result = {}
    for target in targets:
        tmp = [(x, info[0]) for x, info in min_step_data.items() if target in info[1]]
        result[target] = dict(sorted(tmp, key=lambda x: (x[1], x[0])))
    return result


def build_graph(nodes: dict[Layout, int]) -> ig.Graph:
    edges = []
    for code, step in nodes.items():
        for next_case in code.next_cases():
            if next_case not in nodes:
                continue
            assert step - nodes[next_case] in [-1, 0, 1]
            if nodes[next_case] - step == 1:
                edges.append((code, next_case))

    graph = ig.Graph(n=len(nodes))
    graph.vs['code'] = [x.to_string(shorten=False)[:7] for x in nodes]
    graph.vs['step'] = list(nodes.values())
    nodes_index = {y: x for x, y in enumerate(nodes)}
    graph.add_edges([(nodes_index[x], nodes_index[y]) for x, y in edges])
    return graph


def save_graph(name: str, graph: ig.Graph) -> None:
    with open(f'{name}.pkl', 'wb') as fp:
        pickle.dump(graph, fp)


def build_data(group: Group) -> None:
    print(f'Start building: {group}')
    targets = [str(x)[:7] for x in list(group.cases()) if is_valid_solution(x)]
    if not targets:
        return
    data = build_min_step_scope(group, targets)
    for target in targets:
        save_graph(target, build_graph(data[Layout(target)]))


def main() -> None:
    group = GroupUnion(169).groups()[1]
    build_data(group)

    # type_ids = [149, 154, 159, 164, 169, 174]
    # groups = [y for x in type_ids for y in GroupUnion(x).groups()]
    # for group in groups:
    #     build_data(group)


if __name__ == '__main__':
    main()
