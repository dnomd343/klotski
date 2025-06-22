#!/usr/bin/env python3

import json
from klotski import Layout, Block


def is_solution(layout: Layout) -> bool:
    return str(layout).startswith('D')


def is_valid_solution(layout: Layout) -> bool:
    if not is_solution(layout):
        return False

    layout_seq = layout.dump_seq()
    is_z_solution = layout_seq[9] == Block.SPACE and layout_seq[10] == Block.SPACE
    is_x_solution = layout_seq[12] == Block.SPACE and layout_seq[16] == Block.SPACE
    is_y_solution = layout_seq[15] == Block.SPACE and layout_seq[19] == Block.SPACE
    return is_x_solution or is_y_solution or is_z_solution


def get_group_info(group_case: str) -> dict:
    group = Layout(group_case).group
    solutions = [x for x in list(group.cases()) if is_solution(x)]
    valid_solutions = [x for x in solutions if is_valid_solution(x)]
    invalid_solutions = [x for x in solutions if not is_valid_solution(x)]

    return {
        'real_name': str(group),
        'group_size': group.size,
        'solutions': {
            'valid': [str(x)[:7] for x in valid_solutions],
            'invalid': [str(x)[:7] for x in invalid_solutions],
            'valid_num': len(valid_solutions),
            'invalid_num': len(invalid_solutions),
            'total_num': len(solutions),
        }
    }


def do_split(file: str) -> dict:
    raw = json.loads(open(file).read())
    groups = set(x.rsplit('-', maxsplit=1)[0] for x in raw.values())

    data = {}
    for group in sorted(groups):
        targets = [x for x, y in raw.items() if y.startswith(group)]
        group_info = get_group_info(targets[0])
        assert sorted(targets) == group_info['solutions']['valid']

        data[group] = {
            **group_info,
            'targets': {raw[x].removeprefix(f'{group}-'): x for x in targets}
        }
    return data


if __name__ == '__main__':
    with open('data.json', 'w') as fp:
        json.dump(do_split('../classic/classic.json'), fp, indent=2)
