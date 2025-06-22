#!/usr/bin/env python3

import os
import json


def format_1(raw: str) -> str:
    graph = json.loads(raw)
    assert sorted(graph) == list(graph)
    for layout, info in graph.items():
        info['next'] = sorted(info['next'])
    return json.dumps(graph)


def format_2(raw: str) -> str:
    graph = json.loads(raw)['graph']
    for layout, info in graph.items():
        graph[layout] = {'step': info['step'], 'next': info['next']}
    return json.dumps(graph)


def compare(file_1: str, file_2: str) -> None:
    print(f'{file_1} vs {file_2}')
    data_1 = format_1(open(file_1).read())
    data_2 = format_2(open(file_2).read())
    assert data_1 == data_2


if __name__ == '__main__':
    for name in sorted(os.listdir('output-json')):
        compare(f'output/{name}', f'output-json/{name}')
