#!/usr/bin/env python3

import re
import json


def split_item(raw: str) -> list[tuple[str, int]]:
    assert raw[0] == '\n'
    matched = [re.match(r'^([\dA-F]{9}) \((\d+)\)$', x) for x in raw[1:].splitlines()]
    return [(x[1], int(x[2]) - 1) for x in matched]


def load_file(file_name: str) -> dict[str, dict]:
    raw = open(file_name).read()
    assert raw[0] == '['
    assert raw[-1] == '\n'

    result = {}
    for item in raw[1:-1].split('\n['):
        item = item.split('--------')
        assert len(item) == 7
        assert item[-1] == ''
        assert item[2] == item[4]
        assert item[2] == item[5]
        code = re.match(r'^([\dA-F]{9})]\n$', item[0])[1]

        min_solutions = split_item(item[1])
        assert len(min_solutions) in [0, 1]
        solutions = split_item(item[3])
        assert len(set([x[1] for x in solutions])) in [0, 1]
        assert len(set([x[0] for x in solutions])) == len(solutions)

        if not min_solutions:
            min_step = -1
            assert len(solutions) == 0
        else:
            min_step = solutions[0][1]
            assert min_solutions[0] in solutions

        furthest = split_item(item[2])
        assert len(set([x[1] for x in furthest])) == 1
        assert len(set([x[0] for x in furthest])) == len(furthest)

        result[code] = {
            'min_step': min_step,
            'max_step': furthest[0][1],
            'solutions': [x[0] for x in solutions],
            'furthest': [x[0] for x in furthest],
        }
    return result


def load_all(files: list[str]) -> dict[str, dict]:
    data = {}
    [data.update(load_file(x)) for x in files]
    data = {x: data[x] for x in sorted(data)}
    return data


if __name__ == '__main__':
    content = json.dumps(load_all([
        'data_149.txt', 'data_154.txt', 'data_159.txt',
        'data_164.txt', 'data_169.txt', 'data_174.txt'
    ]))
    print(content)
