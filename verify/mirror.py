#!/usr/bin/env python3

import yaml
from functools import cache
from klotski import Group, GroupUnion


def load_data(path: str) -> dict[str, list[int]]:
    raw = yaml.safe_load(open(path).read())
    return {x: y for x, y in raw.items() if len(y)}


@cache
def klotski_groups() -> dict[str, Group]:
    groups = []
    for type_id in range(203):
        groups.extend(GroupUnion(type_id).groups())
    return {str(x): x for x in groups}


def verify_group(name: str, num_1: int, num_2: int, num_3: int) -> None:
    group = klotski_groups()[name]
    assert num_1 + num_2 == num_3
    assert num_3 == group.size

    is_v_mirror = group.is_vertical_mirror
    is_h_mirror = group.is_horizontal_mirror
    is_d_mirror = str(group) == str(group.to_vertical_mirror().to_horizontal_mirror())

    if is_v_mirror and is_h_mirror:  # full mirror
        h_mirrors = [x for x in list(group.cases()) if x.is_horizontal_mirror]
        assert num_1 * 4 - len(h_mirrors) == num_3

    elif is_h_mirror:  # horizontal mirror
        h_mirrors = [x for x in list(group.cases()) if x.is_horizontal_mirror]
        assert num_1 - num_2 == len(h_mirrors)

    elif is_v_mirror:  # vertical mirror
        assert num_1 == num_2

    elif is_d_mirror:  # diagonal mirror
        assert num_1 == num_2

    else:  # non-mirror
        assert num_2 == 0


if __name__ == '__main__':
    data = load_data('raw.yaml')
    [verify_group(x, *y) for x, y in data.items()]
