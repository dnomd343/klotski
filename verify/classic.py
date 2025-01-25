#!/usr/bin/env python3

import json
from enum import Enum
import functools
from pprint import pprint
from klotski import Group, Layout


@functools.cache
def global_data() -> dict[str, dict]:
    return json.loads(open('./data/data.json').read())


class Solution:
    class Type(Enum):
        X = 0
        Y = 1
        Z = 2

    @staticmethod
    def __solu_info(layout: str) -> tuple[int, int]:  # (max_step, ethnic_size)
        cases = [x for x, y in global_data().items() if layout in y['solutions']]
        steps = [global_data()[x]['step'] for x in cases]
        assert min(steps) == 0
        return max(steps), len(cases)

    def __init__(self, code: str, solu_type: Type):
        self.__code = code
        self.__type = solu_type
        self.__max_step, self.__ethnic_size = Solution.__solu_info(code)

    @property
    def code(self) -> str:
        return self.__code

    @property
    def type(self) -> Type:
        return self.__type

    @property
    def group(self) -> Group:
        return self.layout.group

    @property
    def layout(self) -> Layout:
        return Layout(self.code)

    @property
    def max_step(self) -> int:
        return self.__max_step

    @property
    def ethnic_size(self) -> int:
        return self.__ethnic_size

    def __repr__(self):
        val = f'{self.ethnic_size}, {self.max_step}'
        return f'{self.code} {self.type.name} ({val})'


class SoluGroup:
    def __init__(self, name: str):
        self.__name = name
        self.__x_solus: list[Solution] = []
        self.__y_solus: list[Solution] = []
        self.__z_solus: list[Solution] = []

    def add_solu(self, solu: Solution) -> None:
        if solu.type == Solution.Type.X:
            self.__x_solus.append(solu)
        elif solu.type == Solution.Type.Y:
            self.__y_solus.append(solu)
        elif solu.type == Solution.Type.Z:
            self.__z_solus.append(solu)

    @property
    def name(self) -> str:
        return self.__name

    @property
    def solus(self) -> list[Solution]:
        return self.__x_solus + self.__y_solus + self.__z_solus

    @property
    def solu_num(self) -> int:
        return len(self.solus)

    @property
    def group(self) -> Group:
        return self.solus[0].group

    @property
    def group_size(self) -> int:
        return self.group.size

    @property
    def ethnic_sizes(self) -> list[int]:
        return sorted([x.ethnic_size for x in self.solus], key=lambda x: -x)

    @property
    def max_ethnic_size(self) -> int:
        return max(self.ethnic_sizes)

    @property
    def min_case(self) -> Layout:
        return min(list(self.group.cases()))

    @property
    def min_solu(self) -> Layout:
        return min(x.layout for x in self.solus)

    def __repr__(self):
        return (f'<SoluGroup {self.name} ({self.group_size}) '
                f'[{self.min_solu}/{self.min_case}] {self.solu_num} -> {self.ethnic_sizes}>')


class SoluPattern:
    def __init__(self, *solu_groups: SoluGroup):
        assert len(solu_groups) in [1, 2]
        if len(solu_groups) == 1:
            self.__is_single = True
            self.__solu_m = solu_groups[0]
        else:
            self.__is_single = False
            self.__solu_l, self.__solu_r = solu_groups  # TODO: confirm l/r
            assert self.__solu_l.name[:-1] == self.__solu_r.name[:-1]
            assert self.__solu_l.solu_num == self.__solu_r.solu_num
            assert self.__solu_l.group_size == self.__solu_r.group_size
            assert self.__solu_l.ethnic_sizes == self.__solu_r.ethnic_sizes

    @property
    def pattern_size(self) -> int:
        if self.__is_single:
            return self.__solu_m.group_size
        return self.__solu_l.group_size * 2

    @property
    def ethnic_sizes(self) -> list[int]:
        if self.__is_single:
            return self.__solu_m.ethnic_sizes
        return self.__solu_l.ethnic_sizes

    @property
    def max_ethnic_size(self) -> int:
        return max(self.ethnic_sizes)

    @property
    def min_case(self) -> Layout:
        if self.__is_single:
            return self.__solu_m.min_case
        else:
            return min(self.__solu_l.min_case, self.__solu_r.min_case)

    @property
    def min_solu(self) -> Layout:
        if self.__is_single:
            return self.__solu_m.min_solu
        else:
            return min(self.__solu_l.min_solu, self.__solu_r.min_solu)

    def __repr__(self):
        if self.__is_single:
            name = self.__solu_m.name
            solu_num = f'{self.__solu_m.solu_num}'
        else:
            name = f'{self.__solu_l.name}|{self.__solu_r.name}'
            solu_num = f'{self.__solu_l.solu_num}*2'
        return (f'<SoluPattern {name} ({self.pattern_size}) '
                f'[{self.min_solu}/{self.min_case}] {solu_num} -> {self.ethnic_sizes}>')


def load_valid_solutions(num: int) -> list[Solution]:
    raw = json.loads(open('./data/valid_solutions.json').read())
    data_x = [Solution(x, Solution.Type.X) for x in raw[f'{num}x']]
    data_y = [Solution(x, Solution.Type.Y) for x in raw[f'{num}y']]
    data_z = [Solution(x, Solution.Type.Z) for x in raw[f'{num}z']]
    return data_x + data_y + data_z


def split_into_groups(solus: list[Solution]) -> list[SoluPattern]:
    groups = {}
    for solu in solus:
        group = str(solu.group)
        assert group[-1] in ['x', 'n', 'u', 'c', 'd']
        if group not in groups:
            groups[group] = SoluGroup(group)
        groups[group].add_solu(solu)

    pattern_tuples = []
    for group in list(groups):
        if group[-1] in ['x', 'n', 'u']:
            pattern_tuples.append((group, ))
        elif group[-1] == 'c':
            pattern_tuples.append((group, f'{group[:-1]}d'))
        elif group[-1] == 'd':
            pattern_tuples.append((f'{group[:-1]}c', group))

    patterns = []
    for pattern_tuple in sorted(set(pattern_tuples)):
        patterns.append(SoluPattern(*[groups[x] for x in pattern_tuple]))

    return sorted(patterns, key=lambda x: (-x.pattern_size, -x.max_ethnic_size, x.min_solu))


def main() -> None:

    # solu = Solution('DAAF4CC', Solution.Type.X)
    # print(solu)

    # for solu_group in split_into_groups(load_valid_solutions(1)):
    #     print(solu_group)

    for num in range(6):  # 0/1/2/3/4/5
        for solu_group in split_into_groups(load_valid_solutions(num)):
            print(solu_group)
        print('-' * 238)


if __name__ == "__main__":
    main()
