#!/usr/bin/env python3

import re
import json
import functools
from klotski import Block, Layout

type Info = tuple[int, int, str, int, str]


@functools.cache
def solu_data() -> dict[str, dict]:
    return json.loads(open('./data/data.json').read())


def solu_ethnic_info(code: str) -> tuple[int, int]:  # (ethnic_size, ethnic_deep)
    cases = [x for x, y in solu_data().items() if code in y['solutions']]
    steps = [solu_data()[x]['step'] for x in cases]
    return len(cases), max(steps)


def is_ethnic_case(code: Layout) -> bool:
    seq = code.dump_seq()
    if seq[13] != Block.B_2x2:
        return True
    if seq[12] == Block.SPACE and seq[16] == Block.SPACE:
        return True
    if seq[15] == Block.SPACE and seq[19] == Block.SPACE:
        return True
    if seq[9] == Block.SPACE and seq[10] == Block.SPACE:
        return True
    return False


def split_name(code: str, name: str) -> Info:
    match = re.match(r'^([0-5])-(\d{2})([MLR])-(\d{3})(X|Y|Z|ZL|ZR)$', name)
    type_id, solu_type = int(match[1]), match[5]
    assert Layout(code).n_1x2 == type_id

    layout_seq = Layout(code).dump_seq()
    assert solu_type[0] in ['X', 'Y', 'Z']
    if solu_type[0] == 'X':
        assert layout_seq[12] == Block.SPACE and layout_seq[16] == Block.SPACE
    elif solu_type[0] == 'Y':
        assert layout_seq[15] == Block.SPACE and layout_seq[19] == Block.SPACE
    elif solu_type[0] == 'Z':
        assert layout_seq[9] == Block.SPACE and layout_seq[10] == Block.SPACE

    return type_id, int(match[2]), match[3], int(match[4]), solu_type


def split_into_groups(data: dict[str, Info]) -> dict[tuple[int, int, str], dict[str, Info]]:
    groups = {}
    for code, info in data.items():
        name = info[0], info[1], info[2]
        if name not in groups:
            groups[name] = {}
        groups[name][code] = info

    for name, codes in groups.items():
        group = Layout(list(codes)[0]).group
        cases = list(group.cases())
        for code in codes:
            assert Layout(code) in cases

        if name[2] == 'M':
            assert group.is_horizontal_mirror
        else:  # L / R
            assert not group.is_horizontal_mirror

        if name[2] == 'L':
            min_solu_l = min(codes.keys())
            min_solu_r = min(groups[(name[0], name[1], 'R')].keys())
            assert min_solu_l < min_solu_r

    return groups


def verify_m_group(data: dict[str, Info]) -> tuple[int, int, int, str]:
    for code, info in data.items():
        assert info[4] in ['X', 'Y', 'Z', 'ZL', 'ZR']
        if info[4] == 'X':
            y_code = str(Layout(code).to_horizontal_mirror())[:7]
            assert data[y_code] == (info[0], info[1], info[2], info[3], 'Y')
        elif info[4] == 'Y':
            x_code = str(Layout(code).to_horizontal_mirror())[:7]
            assert data[x_code] == (info[0], info[1], info[2], info[3], 'X')
        elif info[4] == 'Z':
            assert Layout(code).is_horizontal_mirror
        elif info[4] == 'ZL':
            zr_code = str(Layout(code).to_horizontal_mirror())[:7]
            assert code < zr_code
            assert data[zr_code] == (info[0], info[1], info[2], info[3], 'ZR')
        elif info[4] == 'ZR':
            zl_code = str(Layout(code).to_horizontal_mirror())[:7]
            assert zl_code < code
            assert data[zl_code] == (info[0], info[1], info[2], info[3], 'ZL')

    x_solus = sorted([(x, y[3]) for x, y in data.items() if y[4] == 'X'], key=lambda x: x[1])
    assert [x[1] for x in x_solus] == list(range(len(x_solus)))
    x_data = [(*solu_ethnic_info(x[0]), x[0]) for x in x_solus]
    assert len(set(x_data)) == len(x_data) and x_data == sorted(x_data, key=lambda x: (-x[0], -x[1], x[2]))

    z_solus = sorted([(x, y[3]) for x, y in data.items() if y[4] in {'Z', 'ZL'}], key=lambda x: x[1])
    assert [x[1] for x in z_solus] == list(range(len(z_solus)))
    z_data = [(*solu_ethnic_info(x[0]), x[0]) for x in z_solus]
    assert len(set(z_data)) == len(z_data) and z_data == sorted(z_data, key=lambda x: (-x[0], -x[1], x[2]))

    group = Layout(list(data)[0]).group
    valid_size = len([x for x in list(group.cases()) if is_ethnic_case(x)])
    max_ethnic_size = max(x_data[0][0] if x_data else -1, z_data[0][0] if z_data else -1)
    min_solu = min(list(data))
    return valid_size, group.size, max_ethnic_size, min_solu


def verify_lr_group(l_data: dict[str, Info], r_data: dict[str, Info]) -> tuple[int, int, int, str]:
    assert len(l_data) == len(r_data)
    for code, info in l_data.items():
        assert info[4] in ['X', 'Y', 'Z']
        if info[4] == 'X':
            y_code = str(Layout(code).to_horizontal_mirror())[:7]
            assert r_data[y_code] == (info[0], info[1], 'R', info[3], 'Y')
        elif info[4] == 'Y':
            x_code = str(Layout(code).to_horizontal_mirror())[:7]
            assert r_data[x_code] == (info[0], info[1], 'R', info[3], 'X')
        elif info[4] == 'Z':
            assert not Layout(code).is_horizontal_mirror
            z_code = str(Layout(code).to_horizontal_mirror())[:7]
            assert r_data[z_code] == (info[0], info[1], 'R', info[3], 'Z')
    for code, info in r_data.items():
        assert info[4] in ['X', 'Y', 'Z']
        if info[4] == 'X':
            y_code = str(Layout(code).to_horizontal_mirror())[:7]
            assert l_data[y_code] == (info[0], info[1], 'L', info[3], 'Y')
        elif info[4] == 'Y':
            x_code = str(Layout(code).to_horizontal_mirror())[:7]
            assert l_data[x_code] == (info[0], info[1], 'L', info[3], 'X')
        elif info[4] == 'Z':
            assert not Layout(code).is_horizontal_mirror
            z_code = str(Layout(code).to_horizontal_mirror())[:7]
            assert l_data[z_code] == (info[0], info[1], 'L', info[3], 'Z')

    lx_solus = sorted([(x, y[3]) for x, y in l_data.items() if y[4] == 'X'], key=lambda x: x[1])
    assert [x[1] for x in lx_solus] == list(range(len(lx_solus)))
    lx_data = [(*solu_ethnic_info(x[0]), x[0]) for x in lx_solus]
    assert len(set(lx_data)) == len(lx_data) and lx_data == sorted(lx_data, key=lambda x: (-x[0], -x[1], x[2]))

    rx_solus = sorted([(x, y[3]) for x, y in r_data.items() if y[4] == 'X'], key=lambda x: x[1])
    assert [x[1] for x in rx_solus] == list(range(len(rx_solus)))
    rx_data = [(*solu_ethnic_info(x[0]), x[0]) for x in rx_solus]
    assert len(set(rx_data)) == len(rx_data) and rx_data == sorted(rx_data, key=lambda x: (-x[0], -x[1], x[2]))

    lz_solus = sorted([(x, y[3]) for x, y in l_data.items() if y[4] == 'Z'], key=lambda x: x[1])
    assert [x[1] for x in lz_solus] == list(range(len(lz_solus)))
    lz_data = [(*solu_ethnic_info(x[0]), x[0]) for x in lz_solus]
    assert len(set(lz_data)) == len(lz_data) and lz_data == sorted(lz_data, key=lambda x: (-x[0], -x[1], x[2]))

    l_group = Layout(list(l_data)[0]).group
    r_group = Layout(list(r_data)[0]).group
    assert l_group.size == r_group.size

    l_valid_size = len([x for x in list(l_group.cases()) if is_ethnic_case(x)])
    r_valid_size = len([x for x in list(r_group.cases()) if is_ethnic_case(x)])
    assert l_valid_size == r_valid_size

    max_ethnic_size = max(lx_data[0][0] if lx_data else -1, rx_data[0][0] if rx_data else -1, lz_data[0][0] if lz_data else -1)
    min_solu = min(list(l_data) + list(r_data))
    return l_valid_size * 2, l_group.size * 2, max_ethnic_size, min_solu


def verify_pattern(info: dict[int, tuple[int, int, int, str]]) -> None:
    assert list(info.keys()) == list(range(len(info)))
    info = list(info.values())
    assert info == sorted(info, key=lambda x: (-x[0], -x[1], -x[2], x[3]))


def main() -> None:
    data = json.loads(open('classic.json').read())
    data = {x: split_name(x, y) for x, y in data.items()}
    groups = split_into_groups(data)

    pattern_info = {0: {}, 1: {}, 2: {}, 3: {}, 4: {}, 5: {}}
    for name, group in groups.items():
        if name[2] == 'M':
            pattern_info[name[0]][name[1]] = verify_m_group(group)
        elif name[2] == 'L':
            pattern_info[name[0]][name[1]] = verify_lr_group(group, groups[(name[0], name[1], 'R')])

    verify_pattern(pattern_info[0])
    verify_pattern(pattern_info[1])
    verify_pattern(pattern_info[2])
    verify_pattern(pattern_info[3])
    verify_pattern(pattern_info[4])
    verify_pattern(pattern_info[5])


if __name__ == '__main__':
    main()
