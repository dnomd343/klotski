#!/usr/bin/env python3

import json

data_legacy = json.loads(open('legacy.json').read())
data_next = json.loads(open('data.json').read())

assert len(data_legacy) == len(data_next)

if __name__ == '__main__':
    for info in data_legacy:
        code = f'{info['code']}00'
        info_next = data_next[code]

        assert info['min_solution_step'] == info_next['min_step']
        assert info['farthest_step'] == info_next['max_step']

        assert info['min_solution_case'] == sorted([x[:7] for x in info_next['solutions']])
        assert info['farthest_case'] == sorted([x[:7] for x in info_next['furthest']])
