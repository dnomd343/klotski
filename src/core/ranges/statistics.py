#!/usr/bin/env python3

def load():
    lines = open('tmp.txt').read().splitlines()
    tmp = [x.split(' ') for x in lines]
    data = [(int(x[0]), int(x[1]), int(x[2])) for x in tmp]
    return data


def statis(data: list[tuple[int, int]]):
    # data -> (distance, offset)

    distances = sorted({x[0] for x in data})

    for distance in distances:
        all_offsets = [x[1] for x in data if x[0] == distance]
        offsets = sorted(set(all_offsets))

        # print(distance, offsets)

        # for offset in offsets:
        #     print(offset, all_offsets.count(offset))

        offset_num = {x: all_offsets.count(x) for x in offsets}

        print(f'{distance} -> {offset_num}')


if __name__ == '__main__':
    raw_data = load()

    for head in range(16):
        if head % 4 == 3:
            continue
        print(f'head = {head}')
        statis([(x[1], x[2]) for x in raw_data])
        # statis([(x[1], x[2]) for x in raw_data if x[0] == head])
        print()

"""
0 -> {7: 177, 8: 2619, 9: 21087, 10: 156415, 11: 756356, 12: 1755849, 13: 1981898, 14: 1057653, 15: 212797}
1 -> {9: 18544, 10: 192582, 11: 596880, 12: 785128, 13: 460136, 14: 98136}
2 -> {8: 3578, 9: 15480, 10: 18792, 11: 8920, 12: 1760, 13: 120}
3 -> {7: 498, 8: 1963, 9: 2215, 10: 1000, 11: 192, 12: 13}
4 -> {8: 67}
5 -> {8: 1722, 9: 42570, 10: 178524, 11: 275405, 12: 178288, 13: 40830}
8 -> {7: 8}
10 -> {7: 490, 8: 5889, 9: 11075, 10: 7000, 11: 1728, 12: 143}
13 -> {6: 1}
15 -> {8: 7852, 9: 57590, 10: 120000, 11: 96384, 12: 26468}
16 -> {6: 50, 7: 433, 8: 625, 9: 297, 10: 53, 11: 3}
32 -> {7: 866, 8: 2500, 9: 1782, 10: 424, 11: 30}
42 -> {7: 23}
43 -> {7: 410, 8: 6875, 9: 16929, 10: 13091, 11: 3039}
57 -> {6: 189, 7: 457, 8: 295, 9: 68, 10: 5}
95 -> {6: 7}
98 -> {6: 182, 7: 1371, 8: 1475, 9: 476, 10: 45}
119 -> {7: 1828, 8: 7670, 9: 8160, 10: 2510}
184 -> {5: 1}
190 -> {5: 52, 6: 276, 7: 221, 8: 47, 9: 2}
284 -> {6: 552, 7: 884, 8: 282, 9: 16}
326 -> {6: 10}
327 -> {6: 266, 7: 2431, 8: 2679, 9: 494}
591 -> {5: 162, 6: 269, 7: 112, 8: 13}
806 -> {5: 6}
810 -> {5: 156, 6: 807, 7: 560, 8: 91}
895 -> {6: 1076, 7: 2912, 8: 1560}
1784 -> {4: 18, 5: 73, 6: 32, 7: 3}
2276 -> {5: 146, 6: 128, 7: 18}
2446 -> {5: 9}
2447 -> {5: 64, 6: 352, 7: 171}
5245 -> {4: 51, 5: 33, 6: 5}
6341 -> {4: 1}
6346 -> {4: 50, 5: 99, 6: 25}
6687 -> {5: 132, 6: 130}
15147 -> {3: 1}
15162 -> {3: 20, 4: 33, 5: 2}
17588 -> {4: 66, 5: 8}
18270 -> {4: 2}
18271 -> {4: 31, 5: 22}
43243 -> {3: 31, 4: 13}
48548 -> {3: 3}
48554 -> {3: 28, 4: 39}
49919 -> {4: 52}
122103 -> {2: 1}
122124 -> {2: 5, 3: 3}
133652 -> {3: 6}
136382 -> {3: 2}
136383 -> {3: 1}
342265 -> {2: 5}
367139 -> {2: 2}
367146 -> {2: 3}
953634 -> {1: 1}
953662 -> {1: 1}
"""

"""
head = 0
0 -> {7: 24, 8: 357, 9: 2834, 10: 19468, 11: 85928, 12: 181284, 13: 185611, 14: 89839, 15: 16369}
1 -> {9: 2548, 10: 23998, 11: 67680, 12: 80822, 13: 42756, 14: 8178}
2 -> {8: 460, 9: 1932, 10: 2262, 11: 1024, 12: 190, 13: 12}
3 -> {7: 46, 8: 161, 9: 177, 10: 79, 11: 15, 12: 1}
4 -> {8: 8}
5 -> {8: 222, 9: 5313, 10: 21489, 11: 31616, 12: 19247, 13: 4083}
10 -> {7: 46, 8: 483, 9: 885, 10: 553, 11: 135, 12: 11}
15 -> {8: 644, 9: 4602, 10: 9480, 11: 7530, 12: 2036}
16 -> {6: 2, 7: 20, 8: 25, 9: 9, 10: 1}
32 -> {7: 40, 8: 100, 9: 54, 10: 8}
43 -> {7: 20, 8: 275, 9: 513, 10: 247}
57 -> {6: 13, 7: 25, 8: 10, 9: 1}
98 -> {6: 13, 7: 75, 8: 50, 9: 7}
119 -> {7: 100, 8: 260, 9: 120}
190 -> {5: 5, 6: 23, 7: 18, 8: 3}
284 -> {6: 46, 7: 72, 8: 18}
327 -> {6: 23, 7: 198, 8: 171}
591 -> {5: 11, 6: 18, 7: 8, 8: 1}
810 -> {5: 11, 6: 54, 7: 40, 8: 7}
895 -> {6: 72, 7: 208, 8: 120}
1784 -> {5: 2, 6: 1}
2276 -> {5: 4, 6: 4}
2447 -> {5: 2, 6: 11}
15162 -> {3: 1, 4: 1}
17588 -> {4: 2}
18271 -> {4: 1}
43243 -> {3: 3, 4: 1}
48554 -> {3: 3, 4: 3}
49919 -> {4: 4}
122124 -> {2: 1}
342265 -> {2: 1}
367146 -> {2: 1}

head = 1
0 -> {7: 9, 8: 165, 9: 1464, 10: 10920, 11: 54215, 12: 130359, 13: 151147, 14: 81673, 15: 16369}
1 -> {9: 1320, 10: 14456, 11: 47280, 12: 64758, 13: 38684, 14: 8178}
2 -> {8: 226, 9: 1104, 10: 1518, 11: 800, 12: 170, 13: 12}
3 -> {7: 21, 8: 88, 9: 113, 10: 59, 11: 13, 12: 1}
4 -> {8: 8}
5 -> {8: 105, 9: 3036, 10: 14421, 11: 24700, 12: 17221, 13: 4083}
10 -> {7: 21, 8: 264, 9: 565, 10: 413, 11: 117, 12: 11}
15 -> {8: 352, 9: 2938, 10: 7080, 11: 6526, 12: 2036}
16 -> {6: 1, 7: 12, 8: 18, 9: 8, 10: 1}
32 -> {7: 24, 8: 72, 9: 48, 10: 8}
43 -> {7: 12, 8: 198, 9: 456, 10: 247}
57 -> {6: 7, 7: 15, 8: 8, 9: 1}
98 -> {6: 7, 7: 45, 8: 40, 9: 7}
119 -> {7: 60, 8: 208, 9: 120}
190 -> {5: 2, 6: 12, 7: 12, 8: 3}
284 -> {6: 24, 7: 48, 8: 18}
327 -> {6: 12, 7: 132, 8: 171}
591 -> {5: 4, 6: 10, 7: 6, 8: 1}
810 -> {5: 4, 6: 30, 7: 30, 8: 7}
895 -> {6: 40, 7: 156, 8: 120}
15162 -> {4: 1}
17588 -> {4: 2}
18271 -> {4: 1}
43243 -> {3: 2, 4: 1}
48554 -> {3: 2, 4: 3}
49919 -> {4: 4}
122124 -> {2: 1, 3: 1}
133652 -> {3: 2}
136383 -> {3: 1}
342265 -> {2: 1}
367146 -> {2: 1}
953662 -> {1: 1}

head = 2
0 -> {7: 24, 8: 359, 9: 2838, 10: 19461, 11: 85764, 12: 181037, 13: 185611, 14: 89839, 15: 16369}
1 -> {9: 2556, 10: 24076, 11: 67800, 12: 80822, 13: 42756, 14: 8178}
2 -> {8: 460, 9: 1932, 10: 2262, 11: 1024, 12: 190, 13: 12}
3 -> {7: 44, 8: 165, 9: 179, 10: 79, 11: 15, 12: 1}
4 -> {8: 8}
5 -> {8: 222, 9: 5313, 10: 21489, 11: 31616, 12: 19247, 13: 4083}
10 -> {7: 44, 8: 495, 9: 895, 10: 553, 11: 135, 12: 11}
15 -> {8: 660, 9: 4654, 10: 9480, 11: 7530, 12: 2036}
16 -> {6: 2, 7: 19, 8: 27, 9: 10, 10: 1}
32 -> {7: 38, 8: 108, 9: 60, 10: 8}
43 -> {7: 19, 8: 297, 9: 570, 10: 247}
57 -> {6: 14, 7: 23, 8: 9, 9: 1}
98 -> {6: 14, 7: 69, 8: 45, 9: 7}
119 -> {7: 92, 8: 234, 9: 120}
190 -> {5: 5, 6: 23, 7: 18, 8: 3}
284 -> {6: 46, 7: 72, 8: 18}
327 -> {6: 23, 7: 198, 8: 171}
591 -> {5: 7, 6: 16, 7: 8, 8: 1}
810 -> {5: 7, 6: 48, 7: 40, 8: 7}
895 -> {6: 64, 7: 208, 8: 120}
5245 -> {4: 2, 5: 1}
6346 -> {4: 2, 5: 3}
6687 -> {5: 4}
15162 -> {3: 1, 4: 1}
17588 -> {4: 2}
18271 -> {4: 1}
43243 -> {3: 3, 4: 1}
48554 -> {3: 3, 4: 3}
49919 -> {4: 4}
122124 -> {2: 1}
342265 -> {2: 1}
367146 -> {2: 1}

head = 4
0 -> {7: 13, 8: 186, 9: 1638, 10: 12049, 11: 58254, 12: 135573, 13: 153173, 14: 81673, 15: 16369}
1 -> {9: 1464, 10: 15808, 11: 50040, 12: 66264, 13: 38684, 14: 8178}
2 -> {8: 244, 9: 1172, 10: 1584, 11: 816, 12: 170, 13: 12}
3 -> {7: 21, 8: 92, 9: 115, 10: 59, 11: 13, 12: 1}
5 -> {8: 122, 9: 3223, 10: 15048, 11: 25194, 12: 17221, 13: 4083}
10 -> {7: 21, 8: 276, 9: 575, 10: 413, 11: 117, 12: 11}
15 -> {8: 368, 9: 2990, 10: 7080, 11: 6526, 12: 2036}
16 -> {7: 4, 8: 12, 9: 7, 10: 1}
32 -> {7: 8, 8: 48, 9: 42, 10: 8}
43 -> {7: 4, 8: 132, 9: 399, 10: 247}
190 -> {5: 2, 6: 8, 7: 6, 8: 1}
284 -> {6: 16, 7: 24, 8: 6}
327 -> {6: 8, 7: 66, 8: 57}
591 -> {5: 7, 6: 16, 7: 8, 8: 1}
810 -> {5: 7, 6: 48, 7: 40, 8: 7}
895 -> {6: 64, 7: 208, 8: 120}
1784 -> {4: 1, 5: 3, 6: 1}
2276 -> {5: 6, 6: 4}
2447 -> {5: 3, 6: 11}
5245 -> {4: 4, 5: 5, 6: 1}
6346 -> {4: 4, 5: 15, 6: 5}
6687 -> {5: 20, 6: 26}
15162 -> {3: 1, 4: 1}
17588 -> {4: 2}
18271 -> {4: 1}
43243 -> {3: 2, 4: 1}
48554 -> {3: 2, 4: 3}
49919 -> {4: 4}
342265 -> {2: 1}
367139 -> {2: 1}
953634 -> {1: 1}

head = 5
0 -> {7: 5, 8: 124, 9: 1161, 10: 8742, 11: 43440, 12: 105459, 13: 126813, 14: 73507, 15: 16369}
1 -> {9: 1044, 10: 11492, 11: 37800, 12: 53714, 13: 34612, 14: 8178}
2 -> {8: 162, 9: 840, 10: 1176, 11: 648, 12: 150, 13: 12}
3 -> {7: 11, 8: 55, 9: 77, 10: 44, 11: 11, 12: 1}
5 -> {8: 81, 9: 2310, 10: 11172, 11: 20007, 12: 15195, 13: 4083}
10 -> {7: 11, 8: 165, 9: 385, 10: 308, 11: 99, 12: 11}
15 -> {8: 220, 9: 2002, 10: 5280, 11: 5522, 12: 2036}
190 -> {5: 1, 6: 6, 7: 5, 8: 1}
284 -> {6: 12, 7: 20, 8: 6}
327 -> {6: 6, 7: 55, 8: 57}
591 -> {5: 7, 6: 14, 7: 7, 8: 1}
810 -> {5: 7, 6: 42, 7: 35, 8: 7}
895 -> {6: 56, 7: 182, 8: 120}
1784 -> {4: 2, 5: 9, 6: 6, 7: 1}
2276 -> {5: 18, 6: 24, 7: 6}
2447 -> {5: 9, 6: 66, 7: 57}
5245 -> {4: 5, 5: 5, 6: 1}
6346 -> {4: 5, 5: 15, 6: 5}
6687 -> {5: 20, 6: 26}
15162 -> {3: 2, 4: 4, 5: 1}
17588 -> {4: 8, 5: 4}
18271 -> {4: 4, 5: 11}
43243 -> {3: 3, 4: 1}
48548 -> {3: 1}
48554 -> {3: 2, 4: 3}
49919 -> {4: 4}
122103 -> {2: 1}
122124 -> {2: 1, 3: 1}
133652 -> {3: 2}
136382 -> {3: 1}
342265 -> {2: 1}
367139 -> {2: 1}

head = 6
0 -> {7: 13, 8: 189, 9: 1635, 10: 11994, 11: 58832, 12: 137293, 13: 154186, 14: 81673, 15: 16369}
1 -> {9: 1464, 10: 15522, 11: 49200, 12: 65762, 13: 38684, 14: 8178}
2 -> {8: 244, 9: 1172, 10: 1584, 11: 816, 12: 170, 13: 12}
3 -> {7: 13, 8: 68, 9: 101, 10: 57, 11: 13, 12: 1}
5 -> {8: 122, 9: 3223, 10: 15048, 11: 25194, 12: 17221, 13: 4083}
10 -> {7: 13, 8: 204, 9: 505, 10: 399, 11: 117, 12: 11}
15 -> {8: 272, 9: 2626, 10: 6840, 11: 6526, 12: 2036}
57 -> {6: 4, 7: 12, 8: 7, 9: 1}
98 -> {6: 4, 7: 36, 8: 35, 9: 7}
119 -> {7: 48, 8: 182, 9: 120}
190 -> {5: 2, 6: 8, 7: 6, 8: 1}
284 -> {6: 16, 7: 24, 8: 6}
327 -> {6: 8, 7: 66, 8: 57}
591 -> {5: 7, 6: 16, 7: 8, 8: 1}
810 -> {5: 7, 6: 48, 7: 40, 8: 7}
895 -> {6: 64, 7: 208, 8: 120}
1784 -> {4: 1, 5: 3, 6: 1}
2276 -> {5: 6, 6: 4}
2447 -> {5: 3, 6: 11}
5245 -> {4: 6, 5: 5, 6: 1}
6341 -> {4: 1}
6346 -> {4: 5, 5: 15, 6: 5}
6687 -> {5: 20, 6: 26}
15147 -> {3: 1}
15162 -> {3: 2, 4: 4}
17588 -> {4: 8}
18270 -> {4: 2}
18271 -> {4: 2}
43243 -> {3: 6, 4: 2}
48548 -> {3: 2}
48554 -> {3: 4, 4: 6}
49919 -> {4: 8}
122124 -> {2: 1, 3: 1}
133652 -> {3: 2}
136382 -> {3: 1}

head = 8
0 -> {7: 18, 8: 204, 9: 1439, 10: 10794, 11: 54722, 12: 131998, 13: 152149, 14: 81673, 15: 16369}
1 -> {9: 1172, 10: 13130, 11: 43200, 12: 59738, 13: 36648, 14: 8178}
2 -> {8: 320, 9: 1368, 10: 1698, 11: 832, 12: 170, 13: 12}
3 -> {7: 30, 8: 133, 9: 163, 10: 77, 11: 15, 12: 1}
4 -> {8: 8}
5 -> {8: 152, 9: 3762, 10: 16131, 11: 25688, 12: 17221, 13: 4083}
10 -> {7: 30, 8: 399, 9: 815, 10: 539, 11: 135, 12: 11}
15 -> {8: 532, 9: 4238, 10: 9240, 11: 7530, 12: 2036}
16 -> {6: 5, 7: 26, 8: 29, 9: 10, 10: 1}
32 -> {7: 52, 8: 116, 9: 60, 10: 8}
43 -> {7: 26, 8: 319, 9: 570, 10: 247}
57 -> {6: 18, 7: 55, 8: 44, 9: 12, 10: 1}
98 -> {6: 18, 7: 165, 8: 220, 9: 84, 10: 9}
119 -> {7: 220, 8: 1144, 9: 1440, 10: 502}
190 -> {5: 2, 6: 8, 7: 6, 8: 1}
284 -> {6: 16, 7: 24, 8: 6}
327 -> {6: 8, 7: 66, 8: 57}
591 -> {5: 17, 6: 23, 7: 9, 8: 1}
806 -> {5: 2}
810 -> {5: 15, 6: 69, 7: 45, 8: 7}
895 -> {6: 92, 7: 234, 8: 120}
1784 -> {4: 3, 5: 12, 6: 3}
2276 -> {5: 24, 6: 12}
2446 -> {5: 5}
2447 -> {5: 7, 6: 33}
5245 -> {4: 11, 5: 8, 6: 1}
6346 -> {4: 11, 5: 24, 6: 5}
6687 -> {5: 32, 6: 26}
15162 -> {3: 3, 4: 6, 5: 1}
17588 -> {4: 12, 5: 4}
18271 -> {4: 6, 5: 11}
43243 -> {3: 2, 4: 1}
48554 -> {3: 2, 4: 3}
49919 -> {4: 4}

head = 9
0 -> {7: 5, 8: 103, 9: 863, 10: 6189, 11: 32410, 12: 85591, 13: 111607, 14: 69424, 15: 16369}
1 -> {9: 748, 10: 8866, 11: 31440, 12: 47690, 13: 32576, 14: 8178}
2 -> {8: 154, 9: 812, 10: 1164, 11: 648, 12: 150, 13: 12}
3 -> {7: 25, 8: 110, 9: 138, 10: 68, 11: 14, 12: 1}
4 -> {8: 8}
5 -> {8: 69, 9: 2233, 10: 11058, 11: 20007, 12: 15195, 13: 4083}
10 -> {7: 25, 8: 330, 9: 690, 10: 476, 11: 126, 12: 11}
15 -> {8: 440, 9: 3588, 10: 8160, 11: 7028, 12: 2036}
16 -> {6: 6, 7: 52, 8: 93, 9: 56, 10: 13, 11: 1}
32 -> {7: 104, 8: 372, 9: 336, 10: 104, 11: 10}
43 -> {7: 52, 8: 1023, 9: 3192, 10: 3211, 11: 1013}
57 -> {6: 19, 7: 58, 8: 45, 9: 12, 10: 1}
98 -> {6: 19, 7: 174, 8: 225, 9: 84, 10: 9}
119 -> {7: 232, 8: 1170, 9: 1440, 10: 502}
190 -> {5: 8, 6: 41, 7: 37, 8: 11, 9: 1}
284 -> {6: 82, 7: 148, 8: 66, 9: 8}
326 -> {6: 2}
327 -> {6: 39, 7: 407, 8: 627, 9: 247}
591 -> {5: 26, 6: 35, 7: 11, 8: 1}
806 -> {5: 2}
810 -> {5: 24, 6: 105, 7: 55, 8: 7}
895 -> {6: 140, 7: 286, 8: 120}
1784 -> {4: 5, 5: 20, 6: 10, 7: 1}
2276 -> {5: 40, 6: 40, 7: 6}
2446 -> {5: 3}
2447 -> {5: 17, 6: 110, 7: 57}
5245 -> {4: 7, 5: 5, 6: 1}
6346 -> {4: 7, 5: 15, 6: 5}
6687 -> {5: 20, 6: 26}
15162 -> {3: 2, 4: 3}
17588 -> {4: 6}
18271 -> {4: 3}
43243 -> {3: 2, 4: 1}
48554 -> {3: 2, 4: 3}
49919 -> {4: 4}

head = 10
0 -> {7: 18, 8: 222, 9: 1694, 10: 10423, 11: 48002, 12: 115822, 13: 137978, 14: 77590, 15: 16369}
1 -> {9: 1536, 10: 16406, 11: 51120, 12: 66766, 13: 38684, 14: 8178}
2 -> {8: 320, 9: 1368, 10: 1698, 11: 832, 12: 170, 13: 12}
3 -> {7: 30, 8: 133, 9: 163, 10: 77, 11: 15, 12: 1}
4 -> {8: 8}
5 -> {8: 152, 9: 3762, 10: 16131, 11: 25688, 12: 17221, 13: 4083}
10 -> {7: 30, 8: 399, 9: 815, 10: 539, 11: 135, 12: 11}
15 -> {8: 532, 9: 4238, 10: 9240, 11: 7530, 12: 2036}
16 -> {6: 5, 7: 26, 8: 29, 9: 10, 10: 1}
32 -> {7: 52, 8: 116, 9: 60, 10: 8}
43 -> {7: 26, 8: 319, 9: 570, 10: 247}
57 -> {6: 26, 7: 65, 8: 46, 9: 12, 10: 1}
98 -> {6: 26, 7: 195, 8: 230, 9: 84, 10: 9}
119 -> {7: 260, 8: 1196, 9: 1440, 10: 502}
190 -> {5: 6, 6: 40, 7: 27, 8: 4}
284 -> {6: 80, 7: 108, 8: 24}
326 -> {6: 4}
327 -> {6: 36, 7: 297, 8: 228}
591 -> {5: 31, 6: 52, 7: 20, 8: 2}
806 -> {5: 2}
810 -> {5: 29, 6: 156, 7: 100, 8: 14}
895 -> {6: 208, 7: 520, 8: 240}
1784 -> {4: 3, 5: 12, 6: 7, 7: 1}
2276 -> {5: 24, 6: 28, 7: 6}
2446 -> {5: 1}
2447 -> {5: 11, 6: 77, 7: 57}
5245 -> {4: 4, 5: 1}
6346 -> {4: 4, 5: 3}
6687 -> {5: 4}
15162 -> {3: 2, 4: 3}
17588 -> {4: 6}
18271 -> {4: 3}
43243 -> {3: 2, 4: 1}
48554 -> {3: 2, 4: 3}
49919 -> {4: 4}

head = 12
0 -> {7: 7, 8: 109, 9: 1467, 10: 14381, 11: 69851, 12: 155802, 13: 168367, 14: 85756, 15: 16369}
1 -> {9: 1368, 10: 15236, 11: 49080, 12: 65762, 13: 38684, 14: 8178}
2 -> {8: 104, 9: 360, 10: 330, 11: 104, 12: 10}
3 -> {7: 53, 8: 199, 9: 210, 10: 89, 11: 16, 12: 1}
5 -> {8: 52, 9: 990, 10: 3135, 11: 3211, 12: 1013}
10 -> {7: 53, 8: 597, 9: 1050, 10: 623, 11: 144, 12: 11}
15 -> {8: 796, 9: 5460, 10: 10680, 11: 8032, 12: 2036}
16 -> {6: 6, 7: 69, 8: 85, 9: 30, 10: 3}
32 -> {7: 138, 8: 340, 9: 180, 10: 24}
42 -> {7: 7}
43 -> {7: 62, 8: 935, 9: 1710, 10: 741}
57 -> {6: 46, 7: 110, 8: 69, 9: 15, 10: 1}
95 -> {6: 5}
98 -> {6: 41, 7: 330, 8: 345, 9: 105, 10: 9}
119 -> {7: 440, 8: 1794, 9: 1800, 10: 502}
184 -> {5: 1}
190 -> {5: 9, 6: 53, 7: 48, 8: 13, 9: 1}
284 -> {6: 106, 7: 192, 8: 78, 9: 8}
326 -> {6: 4}
327 -> {6: 49, 7: 528, 8: 741, 9: 247}
591 -> {5: 15, 6: 23, 7: 9, 8: 1}
810 -> {5: 15, 6: 69, 7: 45, 8: 7}
895 -> {6: 92, 7: 234, 8: 120}
1784 -> {4: 1, 5: 4, 6: 1}
2276 -> {5: 8, 6: 4}
2447 -> {5: 4, 6: 11}
5245 -> {4: 4, 5: 1}
6346 -> {4: 4, 5: 3}
6687 -> {5: 4}
15162 -> {3: 2, 4: 3}
17588 -> {4: 6}
18271 -> {4: 3}
43243 -> {3: 2, 4: 1}
48554 -> {3: 2, 4: 3}
49919 -> {4: 4}

head = 13
0 -> {7: 21, 8: 281, 9: 1934, 10: 16983, 11: 94877, 12: 239808, 13: 286889, 14: 159250, 15: 32738}
1 -> {9: 1516, 10: 16068, 11: 50280, 12: 66264, 13: 38684, 14: 8178}
2 -> {8: 424, 9: 1768, 10: 2046, 11: 936, 12: 180, 13: 12}
3 -> {7: 90, 8: 315, 9: 303, 10: 114, 11: 18, 12: 1}
4 -> {8: 3}
5 -> {8: 209, 9: 4862, 10: 19437, 11: 28899, 12: 18234, 13: 4083}
8 -> {7: 4}
10 -> {7: 86, 8: 945, 9: 1515, 10: 798, 11: 162, 12: 11}
13 -> {6: 1}
15 -> {8: 1260, 9: 7878, 10: 13680, 11: 9036, 12: 2036}
16 -> {6: 14, 7: 128, 8: 188, 9: 92, 10: 17, 11: 1}
32 -> {7: 256, 8: 752, 9: 552, 10: 136, 11: 10}
42 -> {7: 12}
43 -> {7: 116, 8: 2068, 9: 5244, 10: 4199, 11: 1013}
57 -> {6: 27, 7: 68, 8: 47, 9: 12, 10: 1}
95 -> {6: 2}
98 -> {6: 25, 7: 204, 8: 235, 9: 84, 10: 9}
119 -> {7: 272, 8: 1222, 9: 1440, 10: 502}
190 -> {5: 5, 6: 27, 7: 19, 8: 3}
284 -> {6: 54, 7: 76, 8: 18}
327 -> {6: 27, 7: 209, 8: 171}
591 -> {5: 15, 6: 23, 7: 9, 8: 1}
810 -> {5: 15, 6: 69, 7: 45, 8: 7}
895 -> {6: 92, 7: 234, 8: 120}
1784 -> {4: 1, 5: 4, 6: 1}
2276 -> {5: 8, 6: 4}
2447 -> {5: 4, 6: 11}
5245 -> {4: 4, 5: 1}
6346 -> {4: 4, 5: 3}
6687 -> {5: 4}
15162 -> {3: 2, 4: 3}
17588 -> {4: 6}
18271 -> {4: 3}
43243 -> {3: 2, 4: 1}
48554 -> {3: 2, 4: 3}
49919 -> {4: 4}

head = 14
0 -> {7: 20, 8: 320, 9: 2120, 10: 15011, 11: 70061, 12: 155823, 13: 168367, 14: 85756, 15: 16369}
1 -> {9: 1808, 10: 17524, 11: 51960, 12: 66766, 13: 38684, 14: 8178}
2 -> {8: 460, 9: 1652, 10: 1470, 11: 440, 12: 40}
3 -> {7: 114, 8: 444, 9: 476, 10: 198, 11: 34, 12: 2}
4 -> {8: 16}
5 -> {8: 214, 9: 4543, 10: 13965, 11: 13585, 12: 4052}
8 -> {7: 4}
10 -> {7: 110, 8: 1332, 9: 2380, 10: 1386, 11: 306, 12: 22}
15 -> {8: 1776, 9: 12376, 10: 23760, 11: 17068, 12: 4072}
16 -> {6: 9, 7: 77, 8: 119, 9: 65, 10: 14, 11: 1}
32 -> {7: 154, 8: 476, 9: 390, 10: 112, 11: 10}
42 -> {7: 4}
43 -> {7: 73, 8: 1309, 9: 3705, 10: 3458, 11: 1013}
57 -> {6: 15, 7: 26, 8: 10, 9: 1}
98 -> {6: 15, 7: 78, 8: 50, 9: 7}
119 -> {7: 104, 8: 260, 9: 120}
190 -> {5: 5, 6: 27, 7: 19, 8: 3}
284 -> {6: 54, 7: 76, 8: 18}
327 -> {6: 27, 7: 209, 8: 171}
591 -> {5: 15, 6: 23, 7: 9, 8: 1}
810 -> {5: 15, 6: 69, 7: 45, 8: 7}
895 -> {6: 92, 7: 234, 8: 120}
1784 -> {4: 1, 5: 4, 6: 1}
2276 -> {5: 8, 6: 4}
2447 -> {5: 4, 6: 11}
5245 -> {4: 4, 5: 1}
6346 -> {4: 4, 5: 3}
6687 -> {5: 4}
15162 -> {3: 2, 4: 3}
17588 -> {4: 6}
18271 -> {4: 3}
43243 -> {3: 2, 4: 1}
48554 -> {3: 2, 4: 3}
49919 -> {4: 4}
"""
