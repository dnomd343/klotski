#!/usr/bin/env python3

import os
import pickle
import igraph as ig


def load_graph(name: str) -> ig.Graph:
    # with open(os.path.join('DAA7F30', f'{name}.pkl'), 'rb') as fp:
    with open(os.path.join('data', f'{name}.pkl'), 'rb') as fp:
        graph = pickle.load(fp)
        # graph.vs['color'] = ['yellow' for _ in range(graph.vcount())]
        # graph.vs[0]['color'] = 'red'
    return graph


# def save_graph(name: str, g: ig.Graph) -> None:
#     with open(os.path.join('DAA7F30', f'{name}.pkl'), 'wb') as fp:
#         pickle.dump(g, fp)


# def build_stage_1(g: ig.Graph) -> tuple[ig.Graph, ig.Graph]:
#     n1, n2 = tuple(g.neighbors(5999))
#     g.vs[n1]['color'] = 'blue'
#     g.vs[n2]['color'] = 'blue'
#     g.delete_vertices(5999)
#     sub_g = g.decompose()
#     return sub_g[0], sub_g[1]


def demo(g: ig.Graph):
    g.delete_vertices(g.vs['code'].index('9C2BE9C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 11761 and sub.vcount() == 726

    g.delete_vertices(g.vs['code'].index('0BA72F0'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 11462 and sub.vcount() == 298

    g.delete_vertices(g.vs['code'].index('1AAF0F4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 11196 and sub.vcount() == 265

    g.delete_vertices(g.vs['code'].index('0A9F2F0'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 10963 and sub.vcount() == 232

    g.delete_vertices(g.vs['code'].index('033AADC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 10830 and sub.vcount() == 132

    g.delete_vertices(g.vs['code'].index('0AE7E0C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 10726 and sub.vcount() == 103

    g.delete_vertices(g.vs['code'].index('8AA337C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 10625 and sub.vcount() == 100

    g.delete_vertices(g.vs['code'].index('2CCAFA4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 10530 and sub.vcount() == 94

    g.delete_vertices(g.vs['code'].index('2ACB8DC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 10444 and sub.vcount() == 85

    g.delete_vertices(g.vs['code'].index('1BE0B9C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 10362 and sub.vcount() == 81

    g.delete_vertices(g.vs['code'].index('2AFB8C4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 10283 and sub.vcount() == 78

    g.delete_vertices(g.vs['code'].index('2EA3C9C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 10208 and sub.vcount() == 74

    g.delete_vertices(g.vs['code'].index('8AECC9C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 10135 and sub.vcount() == 72

    g.delete_vertices(g.vs['code'].index('CEAF0B4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 10064 and sub.vcount() == 70

    g.delete_vertices(g.vs['code'].index('2EF2E24'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 9995 and sub.vcount() == 68

    g.delete_vertices(g.vs['code'].index('8ABC87C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 9934 and sub.vcount() == 60

    g.delete_vertices(g.vs['code'].index('9BE0A7C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 9876 and sub.vcount() == 57

    g.delete_vertices(g.vs['code'].index('2E8AC7C'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 9791 and sub_1.vcount() == 35 and sub_2.vcount() == 49

    g.delete_vertices(g.vs['code'].index('1F8B2E4'))
    g, sub_1, sub_2, sub_3 = tuple(g.decompose())
    assert g.vcount() == 9735 and sub_1.vcount() == 2 and sub_2.vcount() == 5 and sub_3.vcount() == 48

    g.delete_vertices(g.vs['code'].index('2C8EEB4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 9683 and sub.vcount() == 51

    g.delete_vertices(g.vs['code'].index('CABF0B4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 9635 and sub.vcount() == 47

    g.delete_vertices(g.vs['code'].index('8ABCC9C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 9588 and sub.vcount() == 46

    g.delete_vertices(g.vs['code'].index('2FB82E4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 9541 and sub.vcount() == 46

    g.delete_vertices(g.vs['code'].index('2C8BADC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 9494 and sub.vcount() == 46

    g.delete_vertices(g.vs['code'].index('0ADA33C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 9448 and sub.vcount() == 45

    g.delete_vertices(g.vs['code'].index('2C8FAB4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 9403 and sub.vcount() == 44

    g.delete_vertices(g.vs['code'].index('283ABF4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 9360 and sub.vcount() == 42

    g.delete_vertices(g.vs['code'].index('2BE83B4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 9317 and sub.vcount() == 42

    g.delete_vertices(g.vs['code'].index('8BACC9C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 9275 and sub.vcount() == 41

    g.delete_vertices(g.vs['code'].index('2E8BCB4'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 9203 and sub_1.vcount() == 41 and sub_2.vcount() == 30

    g.delete_vertices(g.vs['code'].index('2ABE0DC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 9163 and sub.vcount() == 39

    g.delete_vertices(g.vs['code'].index('1B3B3A4'))
    g, sub_1, sub_2, sub_3 = tuple(g.decompose())
    assert g.vcount() == 9122 and sub_1.vcount() == 1 and sub_2.vcount() == 1 and sub_3.vcount() == 38

    g.delete_vertices(g.vs['code'].index('1EA2C7C'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 9075 and sub_1.vcount() == 38 and sub_2.vcount() == 8

    g.delete_vertices(g.vs['code'].index('023BAF4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 9021 and sub.vcount() == 53

    g.delete_vertices(g.vs['code'].index('023AEF4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8971 and sub.vcount() == 49

    g.delete_vertices(g.vs['code'].index('0AA33F4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8914 and sub.vcount() == 56

    g.delete_vertices(g.vs['code'].index('AAAF07C'))
    sub, g = tuple(g.decompose())
    assert g.vcount() == 8876 and sub.vcount() == 37

    g.delete_vertices(g.vs['code'].index('0F0AAF4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8838 and sub.vcount() == 37

    g.delete_vertices(g.vs['code'].index('0EE28F4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8800 and sub.vcount() == 37

    g.delete_vertices(g.vs['code'].index('022AFDC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8762 and sub.vcount() == 37

    g.delete_vertices(g.vs['code'].index('0CBA8DC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8707 and sub.vcount() == 54

    g.delete_vertices(g.vs['code'].index('0CCEAB4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8654 and sub.vcount() == 52

    g.delete_vertices(g.vs['code'].index('CFE8CA4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8617 and sub.vcount() == 36

    g.delete_vertices(g.vs['code'].index('283AFB4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8580 and sub.vcount() == 36

    g.delete_vertices(g.vs['code'].index('1BAFC90'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8544 and sub.vcount() == 35

    g.delete_vertices(g.vs['code'].index('0AB4BCC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8509 and sub.vcount() == 34

    g.delete_vertices(g.vs['code'].index('283EEE4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8475 and sub.vcount() == 33

    g.delete_vertices(g.vs['code'].index('2F22EE4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8344 and sub.vcount() == 130

    g.delete_vertices(g.vs['code'].index('0A7F8B0'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8311 and sub.vcount() == 32

    g.delete_vertices(g.vs['code'].index('2E0FAB4'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 8279 and sub_1.vcount() == 30 and sub_2.vcount() == 1

    g.delete_vertices(g.vs['code'].index('2A8B3F4'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 8244 and sub_1.vcount() == 5 and sub_2.vcount() == 29

    g.delete_vertices(g.vs['code'].index('CFEA0B4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8216 and sub.vcount() == 27

    g.delete_vertices(g.vs['code'].index('8BBE09C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8188 and sub.vcount() == 27

    g.delete_vertices(g.vs['code'].index('CF2A2F4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8137 and sub.vcount() == 50

    g.delete_vertices(g.vs['code'].index('0BE8CB4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8110 and sub.vcount() == 26

    g.delete_vertices(g.vs['code'].index('1F8B3A4'))
    g, sub_1, sub_2, sub_3 = tuple(g.decompose())
    assert g.vcount() == 8076 and sub_1.vcount() == 6 and sub_2.vcount() == 1 and sub_3.vcount() == 26

    g.delete_vertices(g.vs['code'].index('033EAB4'))
    g, sub_1, sub_2, sub_3, sub_4 = tuple(g.decompose())
    assert g.vcount() == 8044 and sub_1.vcount() == 1 and sub_2.vcount() == 3 and sub_3.vcount() == 1 and sub_4.vcount() == 26

    g.delete_vertices(g.vs['code'].index('0BA83DC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 8017 and sub.vcount() == 26

    g.delete_vertices(g.vs['code'].index('0AA33DC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7990 and sub.vcount() == 26

    g.delete_vertices(g.vs['code'].index('1B8A3DC'))
    g, sub_1, sub_2, sub_3 = tuple(g.decompose())
    assert g.vcount() == 7949 and sub_1.vcount() == 26 and sub_2.vcount() == 7 and sub_3.vcount() == 7

    g.delete_vertices(g.vs['code'].index('283BFA4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7799 and sub.vcount() == 149

    g.delete_vertices(g.vs['code'].index('282BFB4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7662 and sub.vcount() == 136

    g.delete_vertices(g.vs['code'].index('0BE88F4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7552 and sub.vcount() == 109

    g.delete_vertices(g.vs['code'].index('032AEDC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7526 and sub.vcount() == 25

    g.delete_vertices(g.vs['code'].index('8E2CB9C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7501 and sub.vcount() == 24

    g.delete_vertices(g.vs['code'].index('8E3CA9C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7476 and sub.vcount() == 24

    g.delete_vertices(g.vs['code'].index('8EBE09C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7451 and sub.vcount() == 24

    g.delete_vertices(g.vs['code'].index('0E9F8B0'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7426 and sub.vcount() == 24

    g.delete_vertices(g.vs['code'].index('0B9F8B0'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7401 and sub.vcount() == 24

    g.delete_vertices(g.vs['code'].index('0E9B8CC'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 7368 and sub_1.vcount() == 8 and sub_2.vcount() == 24

    g.delete_vertices(g.vs['code'].index('2ACB8F4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7343 and sub.vcount() == 24

    g.delete_vertices(g.vs['code'].index('0A7B8CC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7318 and sub.vcount() == 24

    g.delete_vertices(g.vs['code'].index('2BAEF40'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7293 and sub.vcount() == 24

    g.delete_vertices(g.vs['code'].index('0A43BBC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7238 and sub.vcount() == 54

    g.delete_vertices(g.vs['code'].index('0A07AFC'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 7146 and sub_1.vcount() == 90 and sub_2.vcount() == 1

    g.delete_vertices(g.vs['code'].index('0FF42A8'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7122 and sub.vcount() == 23

    g.delete_vertices(g.vs['code'].index('1A18BFC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7098 and sub.vcount() == 23

    g.delete_vertices(g.vs['code'].index('8AB837C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7076 and sub.vcount() == 21

    g.delete_vertices(g.vs['code'].index('2CCAAF4'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 7045 and sub_1.vcount() == 21 and sub_2.vcount() == 9

    g.delete_vertices(g.vs['code'].index('2A83EDC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7023 and sub.vcount() == 21

    g.delete_vertices(g.vs['code'].index('1BBE8C4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 7001 and sub.vcount() == 21

    g.delete_vertices(g.vs['code'].index('0B2B3B4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6898 and sub.vcount() == 102

    g.delete_vertices(g.vs['code'].index('2BEBB04'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6830 and sub.vcount() == 67

    g.delete_vertices(g.vs['code'].index('1BE8C9C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6766 and sub.vcount() == 63

    g.delete_vertices(g.vs['code'].index('1BF0AB4'))
    g, sub_1, sub_2, sub_3 = tuple(g.decompose())
    assert g.vcount() == 6712 and sub_1.vcount() == 34 and sub_2.vcount() == 3 and sub_3.vcount() == 16

    g.delete_vertices(g.vs['code'].index('2B8E2F4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6692 and sub.vcount() == 19

    g.delete_vertices(g.vs['code'].index('9BAE07C'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 6658 and sub_1.vcount() == 15 and sub_2.vcount() == 18

    g.delete_vertices(g.vs['code'].index('8B0EADC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6639 and sub.vcount() == 18

    g.delete_vertices(g.vs['code'].index('8B0EE9C'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 6618 and sub_1.vcount() == 18 and sub_2.vcount() == 2

    g.delete_vertices(g.vs['code'].index('8B0EFA4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6599 and sub.vcount() == 18

    g.delete_vertices(g.vs['code'].index('1B3B2E4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6581 and sub.vcount() == 17

    g.delete_vertices(g.vs['code'].index('0EABF40'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 6561 and sub_1.vcount() == 17 and sub_2.vcount() == 2

    g.delete_vertices(g.vs['code'].index('1AEF834'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6543 and sub.vcount() == 17

    g.delete_vertices(g.vs['code'].index('1B83AF4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6497 and sub.vcount() == 45

    g.delete_vertices(g.vs['code'].index('8BBC89C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6480 and sub.vcount() == 16

    g.delete_vertices(g.vs['code'].index('2ABBF40'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6463 and sub.vcount() == 16

    g.delete_vertices(g.vs['code'].index('9EAE07C'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 6434 and sub_1.vcount() == 13 and sub_2.vcount() == 15

    g.delete_vertices(g.vs['code'].index('2C8EEE4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6418 and sub.vcount() == 15

    g.delete_vertices(g.vs['code'].index('0BA72CC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6402 and sub.vcount() == 15

    g.delete_vertices(g.vs['code'].index('8EAC87C'))
    sub, g = tuple(g.decompose())
    assert g.vcount() == 6333 and sub.vcount() == 68

    g.delete_vertices(g.vs['code'].index('CA32FB4'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 6315 and sub_1.vcount() == 3 and sub_2.vcount() == 14

    g.delete_vertices(g.vs['code'].index('CE38BB4'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 6297 and sub_1.vcount() == 3 and sub_2.vcount() == 14

    g.delete_vertices(g.vs['code'].index('2BE3CA4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6282 and sub.vcount() == 14

    g.delete_vertices(g.vs['code'].index('0EEB21C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6267 and sub.vcount() == 14

    g.delete_vertices(g.vs['code'].index('0EE88DC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6252 and sub.vcount() == 14

    g.delete_vertices(g.vs['code'].index('0BE8C9C'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 6085 and sub_1.vcount() == 164 and sub_2.vcount() == 2

    g.delete_vertices(g.vs['code'].index('0AF0A7C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6071 and sub.vcount() == 13

    g.delete_vertices(g.vs['code'].index('0B24BBC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6057 and sub.vcount() == 13

    g.delete_vertices(g.vs['code'].index('0BA7B0C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 6043 and sub.vcount() == 13

    g.delete_vertices(g.vs['code'].index('0AACCF4'))
    g, sub_1, sub_2, sub_3 = tuple(g.decompose())
    assert g.vcount() == 6002 and sub_1.vcount() == 1 and sub_2.vcount() == 2 and sub_3.vcount() == 37

    g.delete_vertices(g.vs['code'].index('0A06EFC'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 5976 and sub_1.vcount() == 24 and sub_2.vcount() == 1

    g.delete_vertices(g.vs['code'].index('1A2B3F4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 5953 and sub.vcount() == 22

    g.delete_vertices(g.vs['code'].index('022EEF4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 5932 and sub.vcount() == 20

    g.delete_vertices(g.vs['code'].index('1BCE8B4'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 5757 and sub_1.vcount() == 173 and sub_2.vcount() == 1

    g.delete_vertices(g.vs['code'].index('122BBDC'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 5697 and sub.vcount() == 59

    g.delete_vertices(g.vs['code'].index('1A8F3B4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 5659 and sub.vcount() == 37

    g.delete_vertices(g.vs['code'].index('1BCA8F4'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 5624 and sub_1.vcount() == 19 and sub_2.vcount() == 15

    g.delete_vertices(g.vs['code'].index('0B9AFC0'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 5602 and sub_1.vcount() == 19 and sub_2.vcount() == 2

    g.delete_vertices(g.vs['code'].index('1BF2AC4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 5574 and sub.vcount() == 27

    g.delete_vertices(g.vs['code'].index('8BAC87C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 5561 and sub.vcount() == 12

    g.delete_vertices(g.vs['code'].index('8B0EB9C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 5548 and sub.vcount() == 12

    g.delete_vertices(g.vs['code'].index('6C3FAA4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 5535 and sub.vcount() == 12

    g.delete_vertices(g.vs['code'].index('68F3AE4'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 5520 and sub_1.vcount() == 12 and sub_2.vcount() == 2

    g.delete_vertices(g.vs['code'].index('0BABF40'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 5505 and sub_1.vcount() == 12 and sub_2.vcount() == 2

    g.delete_vertices(g.vs['code'].index('1ABE3C4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 5492 and sub.vcount() == 12

    g.delete_vertices(g.vs['code'].index('2BA33B4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 5479 and sub.vcount() == 12

    g.delete_vertices(g.vs['code'].index('2FAE8C4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 5412 and sub.vcount() == 66

    g.delete_vertices(g.vs['code'].index('8BAE07C'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 5381 and sub.vcount() == 30

    g.delete_vertices(g.vs['code'].index('8BAF21C'))
    g, sub_1, sub_2 = tuple(g.decompose())
    assert g.vcount() == 5365 and sub_1.vcount() == 14 and sub_2.vcount() == 1

    g.delete_vertices(g.vs['code'].index('8BEEB40'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 5317 and sub.vcount() == 47

    g.delete_vertices(g.vs['code'].index('2BEB8C4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 5304 and sub.vcount() == 12

    g.delete_vertices(g.vs['code'].index('2A3B3B4'))
    g, sub = tuple(g.decompose())
    assert g.vcount() == 5287 and sub.vcount() == 16

    g.write_graphml('main.graphml')
    g.write_pickle('main.pkl')

    print(g.vcount(), sub.vcount())

    # ret = g.decompose()
    # print(len(ret))

    points = g.articulation_points()
    print(len(points))

    result = []
    for point in points:
        tmp = g.copy()
        tmp.delete_vertices(point)
        sub_g = tmp.decompose()
        # if 11 in [x.vcount() for x in sub_g]:
        #     print(g.vs[point])
        result.extend([x.vcount() for x in sub_g])
    print(sorted(set(result)))

    # print(point, [x.vcount() for x in sub_g], g.vs[point])


if __name__ == '__main__':
    # ig.plot(g, 'demo.png', bbox=(800, 800), vertex_size=5)

    # stage_1, part_1 = build_stage_1(load_graph('stage_0'))
    # save_graph('stage_1', stage_1)
    # save_graph('part_1', part_1)

    demo(load_graph('DAA7F30'))
