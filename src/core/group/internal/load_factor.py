#!/usr/bin/env python3

import re

# (type_id, pattern_id): (load_factor_a, coff, load_factor_b)
type data_type = dict[tuple[int, int], tuple[float, float, float]]


def load_data(lines: list[str]) -> data_type:
    result = {}
    key, items = (), []
    for line in lines:
        if line.startswith('['):
            match = re.match(r'^\[(\d+), (\d+)]$', line)
            key = (int(match[1]), int(match[2]))
        elif not line:
            assert len(key) == 2
            assert items[0][0] == 1.0
            if len(items) == 1:
                assert items[0][1] < 0.1  # skip low cases
            else:
                assert len(items) == 2
                result[key] = items[0][1], items[1][0], items[1][1]
            key, items = (), []
        else:
            match = re.match(r'^(\d\.\d{2}), (\d\.\d{6})$', line)
            items.append((float(match[1]), float(match[2])))
    return result


def analyse_data(data: data_type) -> None:
    data = {x: y for x, y in data.items() if y[0] >= 0.5}

    times = set([int(x * 1000 / y) / 1000 for x, _, y in data.values()])
    print(sorted(times))

    type_a, type_b, type_c = [], [], []
    for group, (load_factor, coff, _) in data.items():
        if load_factor <= 0.55:
            type_a.append((group, load_factor, coff))
        elif coff <= 1.3:
            type_b.append((group, load_factor, coff))
        else:
            type_c.append((group, load_factor, coff))

    type_c = sorted(type_c, key=lambda x: x[2])
    for item in type_c:
        print(item)

    # ((117, 0), 0.571359, 1.54) -> 4680
    # ((118, 0), 0.571298, 1.54) -> 37440
    # ((133, 0), 0.570803, 1.54) -> 149632
    # ((134, 0), 0.570558, 1.54) -> 299136

    # ((63, 0), 0.568915, 1.55) -> 582
    # ((136, 0), 0.565568, 1.55) -> 296520

    # ((112, 0), 0.563973, 1.56) -> 36960
    # ((113, 0), 0.563969, 1.56) -> 73920

    # ((197, 0), 0.714286, 1.6) -> 5
    # ((197, 1), 0.714286, 1.6) -> 5
    # ((197, 2), 0.714286, 1.6) -> 5
    # ((197, 3), 0.714286, 1.6) -> 5
    # ((197, 4), 0.714286, 1.6) -> 5
    # ((197, 5), 0.714286, 1.6) -> 5


if __name__ == '__main__':
    raw = open('load_factor.txt').read().splitlines()
    analyse_data(load_data(raw))
