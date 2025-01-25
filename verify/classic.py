#!/usr/bin/env python3

import json
import functools
from enum import Enum
from pprint import pprint
from klotski import Block, Group, Layout


@functools.cache
def global_data() -> dict[str, dict]:
    return json.loads(open('./data/data.json').read())


class Solution:
    class Type(Enum):
        X = 0
        Y = 1
        Z = 2

    @staticmethod
    def __solu_info(layout: str) -> tuple[int, int]:  # (ethnic_deep, ethnic_size)
        cases = [x for x, y in global_data().items() if layout in y['solutions']]
        steps = [global_data()[x]['step'] for x in cases]
        assert min(steps) == 0
        return max(steps), len(cases)

    def __init__(self, code: str, solu_type: Type):
        self.__name = ''
        self.__code = code
        self.__type = solu_type
        self.__ethnic_deep, self.__ethnic_size = Solution.__solu_info(code)

    @property
    def code(self) -> str:
        return self.__code

    @property
    def type(self) -> Type:
        return self.__type

    @property
    def name(self) -> str:
        return self.__name

    @name.setter
    def name(self, name: str) -> None:
        self.__name = name

    @property
    def group(self) -> Group:
        return self.layout.group

    @property
    def layout(self) -> Layout:
        return Layout(self.code)

    @property
    def ethnic_deep(self) -> int:
        return self.__ethnic_deep

    @property
    def ethnic_size(self) -> int:
        return self.__ethnic_size

    def __repr__(self):
        return (f'<Solution {self.name} [{self.code}] {self.type.name} '
                f'({self.ethnic_size} {self.ethnic_deep})>')


class SoluGroup:
    def __init__(self, name: str):
        self.__name = ''
        self.__inner_name = name  # klotski group name
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

    @name.setter
    def name(self, name: str) -> None:
        self.__name = name

    @property
    def inner_name(self) -> str:
        return self.__inner_name

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
    def group_valid_size(self) -> int:

        def is_invalid_solu(layout: Layout) -> bool:
            if str(layout)[0] != 'D':  # 2x2 block address not 13
                return False
            block_seq = layout.dump_seq()
            is_type_x = block_seq[12] == Block.SPACE and block_seq[16] == Block.SPACE
            is_type_y = block_seq[15] == Block.SPACE and block_seq[19] == Block.SPACE
            is_type_z = block_seq[9] == Block.SPACE and block_seq[10] == Block.SPACE
            return not (is_type_x or is_type_y or is_type_z)

        num = len([x for x in list(self.group.cases()) if is_invalid_solu(x)])
        return self.group_size - num

    @property
    def ethnic_sizes(self) -> list[int]:
        return sorted([x.ethnic_size for x in self.solus], key=lambda x: -x)

    @property
    def max_ethnic_size(self) -> int:
        return max(x.ethnic_size for x in self.solus)

    @property
    def max_ethnic_deep(self) -> int:
        return max(x.ethnic_deep for x in self.solus)

    @property
    def min_case(self) -> Layout:
        return min(list(self.group.cases()))

    @property
    def min_solu(self) -> Layout:
        return min(x.layout for x in self.solus)

    def __repr__(self):
        return (f'<SoluGroup {self.name} {self.inner_name} ({self.group_valid_size}/{self.group_size}) '
                f'[{self.min_solu}/{self.min_case}] ({self.max_ethnic_deep}) '
                f'{self.solu_num} -> {self.ethnic_sizes}>')

    def build_m_index(self) -> None:
        assert self.__name[-1] == 'M'

        assert len(self.__x_solus) == len(self.__y_solus)
        self.__x_solus.sort(key=lambda x: (-x.ethnic_size, -x.ethnic_deep, x.layout))
        y_index = {str(y.layout.to_horizontal_mirror())[:7]: x for x, y in enumerate(self.__x_solus)}
        self.__y_solus.sort(key=lambda x: y_index[x.code])

        for num in range(len(self.__x_solus)):
            self.__x_solus[num].name = f'{self.name}-{num:03}X'
            self.__y_solus[num].name = f'{self.name}-{num:03}Y'

        z_map = {x.code: x for x in self.__z_solus}
        z_pairs = [tuple(sorted([x.layout, x.layout.to_horizontal_mirror()])) for x in self.__z_solus]
        z_pairs = [(z_map[str(x)[:7]], z_map[str(y)[:7]]) for x, y in set(z_pairs)]
        z_pairs.sort(key=lambda x: (-x[0].ethnic_size, -x[0].ethnic_deep, x[0].layout))

        for num in range(len(z_pairs)):
            if z_pairs[num][0] == z_pairs[num][1]:
                z_pairs[num][0].name = f'{self.name}-{num:03}Z'
            else:
                z_pairs[num][0].name = f'{self.name}-{num:03}ZL'
                z_pairs[num][1].name = f'{self.name}-{num:03}ZR'

    def build_lx_index(self) -> dict[str, str]:
        assert self.__name[-1] == 'L'
        right_data = {}
        self.__x_solus.sort(key=lambda x: (-x.ethnic_size, -x.ethnic_deep, x.layout))
        for num in range(len(self.__x_solus)):
            self.__x_solus[num].name = f'{self.name}-{num:03}X'
            mirror = str(self.__x_solus[num].layout.to_horizontal_mirror())[:7]
            right_data[mirror] = f'{num:03}Y'
        return right_data

    def build_rx_index(self) -> dict[str, str]:
        assert self.__name[-1] == 'R'
        left_data = {}
        self.__x_solus.sort(key=lambda x: (-x.ethnic_size, -x.ethnic_deep, x.layout))
        for num in range(len(self.__x_solus)):
            self.__x_solus[num].name = f'{self.name}-{num:03}X'
            mirror = str(self.__x_solus[num].layout.to_horizontal_mirror())[:7]
            left_data[mirror] = f'{num:03}Y'
        return left_data

    def apply_ly_index(self, data: dict[str, str]) -> None:
        assert self.__name[-1] == 'L'
        for solu in self.__y_solus:
            solu.name = f'{self.name}-{data[solu.code]}'

    def apply_ry_index(self, data: dict[str, str]) -> None:
        assert self.__name[-1] == 'R'
        for solu in self.__y_solus:
            solu.name = f'{self.name}-{data[solu.code]}'

    def build_lz_index(self) -> dict[str, str]:
        assert self.__name[-1] == 'L'
        data = {}
        self.__z_solus.sort(key=lambda x: (-x.ethnic_size, -x.ethnic_deep, x.layout))
        for num in range(len(self.__z_solus)):
            self.__z_solus[num].name = f'{self.name}-{num:03}Z'
            mirror = str(self.__z_solus[num].layout.to_horizontal_mirror())[:7]
            data[mirror] = f'{num:03}Z'
        return data

    def apply_rz_index(self, data: dict[str, str]) -> None:
        assert self.__name[-1] == 'R'
        for solu in self.__z_solus:
            solu.name = f'{self.name}-{data[solu.code]}'


class SoluPattern:
    def __init__(self, *solu_groups: SoluGroup):
        self.__pattern_id = -1
        inner_type_id = solu_groups[0].inner_name[:3]
        self.__type_id = {'174': 0, '169': 1, '164': 2, '159': 3, '154': 4, '149': 5}[inner_type_id]

        assert len(solu_groups) in [1, 2]
        if len(solu_groups) == 1:
            self.__is_single = True
            self.__group_m = solu_groups[0]
        else:
            self.__is_single = False
            if solu_groups[0].min_solu < solu_groups[1].min_solu:
                self.__group_l, self.__group_r = solu_groups
            else:
                self.__group_r, self.__group_l = solu_groups
            assert self.__group_l.inner_name[:-1] == self.__group_r.inner_name[:-1]
            assert self.__group_l.solu_num == self.__group_r.solu_num
            assert self.__group_l.group_size == self.__group_r.group_size
            assert self.__group_l.ethnic_sizes == self.__group_r.ethnic_sizes

    @property
    def type_id(self) -> int:
        return self.__type_id

    @property
    def pattern_id(self) -> int:
        return self.__pattern_id

    @pattern_id.setter
    def pattern_id(self, id_val: int) -> None:
        self.__pattern_id = id_val
        if self.__is_single:
            self.__group_m.name = f'{self.type_id}-{self.pattern_id:02}M'
        else:
            self.__group_l.name = f'{self.type_id}-{self.pattern_id:02}L'
            self.__group_r.name = f'{self.type_id}-{self.pattern_id:02}R'

    @property
    def pattern_size(self) -> int:
        if self.__is_single:
            return self.__group_m.group_size
        return self.__group_l.group_size * 2

    @property
    def pattern_valid_size(self) -> int:
        if self.__is_single:
            return self.__group_m.group_valid_size
        assert self.__group_l.group_valid_size == self.__group_r.group_valid_size
        return self.__group_l.group_valid_size * 2

    @property
    def ethnic_sizes(self) -> list[int]:
        if self.__is_single:
            return self.__group_m.ethnic_sizes
        assert self.__group_l.ethnic_sizes == self.__group_r.ethnic_sizes
        return self.__group_l.ethnic_sizes

    @property
    def max_ethnic_size(self) -> int:
        if self.__is_single:
            return self.__group_m.max_ethnic_size
        assert self.__group_l.max_ethnic_size == self.__group_r.max_ethnic_size
        return self.__group_l.max_ethnic_size

    @property
    def max_ethnic_deep(self) -> int:
        if self.__is_single:
            return self.__group_m.max_ethnic_deep
        assert self.__group_l.max_ethnic_deep == self.__group_r.max_ethnic_deep
        return self.__group_l.max_ethnic_deep

    @property
    def min_case(self) -> Layout:
        if self.__is_single:
            return self.__group_m.min_case
        else:
            return min(self.__group_l.min_case, self.__group_r.min_case)

    @property
    def min_solu(self) -> Layout:
        if self.__is_single:
            return self.__group_m.min_solu
        else:
            return min(self.__group_l.min_solu, self.__group_r.min_solu)

    @property
    def solus(self) -> list[Solution]:
        if self.__is_single:
            return self.__group_m.solus
        else:
            return self.__group_l.solus + self.__group_r.solus

    def __repr__(self):
        if self.__is_single:
            name = self.__group_m.name
            inner_name = self.__group_m.inner_name
            solu_num = f'{self.__group_m.solu_num}'
            group_repr = f'{self.__group_m}'
        else:
            name = f'{self.__group_l.name}|{self.__group_r.name}'
            inner_name = f'{self.__group_l.inner_name}|{self.__group_r.inner_name}'
            solu_num = f'{self.__group_l.solu_num}*2'
            group_repr = f'{self.__group_l}\n{self.__group_r}'
        return (f'<SoluPattern {name} {inner_name} ({self.pattern_valid_size}/{self.pattern_size}) '
                f'[{self.min_solu}/{self.min_case}] ({self.max_ethnic_deep}) '
                f'{solu_num} -> {self.ethnic_sizes}>\n{group_repr}')

    def build_index(self) -> None:
        if self.__is_single:
            self.__group_m.build_m_index()
        else:
            self.__group_r.apply_ry_index(self.__group_l.build_lx_index())
            self.__group_l.apply_ly_index(self.__group_r.build_rx_index())
            self.__group_r.apply_rz_index(self.__group_l.build_lz_index())


def load_valid_solutions(num: int) -> list[Solution]:
    raw = json.loads(open('./data/valid_solutions.json').read())
    data_x = [Solution(x, Solution.Type.X) for x in raw[f'{num}x']]
    data_y = [Solution(x, Solution.Type.Y) for x in raw[f'{num}y']]
    data_z = [Solution(x, Solution.Type.Z) for x in raw[f'{num}z']]
    return data_x + data_y + data_z


def split_patterns(solus: list[Solution]) -> list[SoluPattern]:
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

    patterns.sort(key=lambda x: (-x.pattern_valid_size, -x.pattern_size, -x.max_ethnic_size, x.min_solu))
    for pattern_id, pattern in enumerate(patterns):
        pattern.pattern_id = pattern_id
        pattern.build_index()
    return patterns


def main() -> None:
    solus = []
    for num in range(6):  # 0/1/2/3/4/5
        for solu_pattern in split_patterns(load_valid_solutions(num)):
            solus.extend(solu_pattern.solus)
            # print(solu_pattern, end='\n\n')
        # print('-' * 238)

    data = sorted([(x.code, x.name) for x in solus], key=lambda x: x[1])
    print(json.dumps(dict(data)))


if __name__ == "__main__":
    main()
