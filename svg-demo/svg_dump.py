#!/usr/bin/env python3
# -*- coding: utf-8 -*-

CASE_WIDTH = 24
CASE_HEIGHT = 36

CASE_DISTANCE_X = 40
CASE_DISTANCE_Y = 20

MAIN_MARGIN_X = 10
MAIN_MARGIN_Y = 8

layer = [
    ["603EDF5CAFFF5E2"], # layer 0
    ["61BEC75CAFFF5E2","0C3EDF5CAFFF5E2"], # layer 1
    ["0DBEC75CAFFF5E2","EC34DF1CAFFF5E2"], # layer 2
    ["EDB4C71CAFFF5E2","EC34DFE42FFF5E2"], # layer 3
    ["EDB4C7E42FFF5E2"], # layer 4
    ["EDB4F8E50FFF5E2"], # layer 5
]

link = [[
    [0, 1] # (0, 0) -> 1
], [
    [0], # (1, 0) -> 2
    [0, 1] # (1, 1) -> 2
], [
    [0], # (2, 0) -> 3
    [0, 1], # (2, 1) -> 3
], [
    [0], # (3, 0) -> 4
    [0], # (3, 1) -> 4
], [
    [0], # (4, 0) -> 5
]]

# print(layer)
# print(link)

MAX_CASE_X = 2
MAX_CASE_Y = 6


# def cal_length(num: int, single_length: int, distance: int):
#     return num * single_length + (num - 1) * distance


MAX_WIDTH = MAX_CASE_X * CASE_WIDTH + (MAX_CASE_X - 1) * CASE_DISTANCE_X

def case_address(layer_num: int, layer_index: int) -> (int, int):

    case_number_x = len(layer[layer_num])

    MY_WIDTH = case_number_x * CASE_WIDTH + (case_number_x - 1) * CASE_DISTANCE_X

    MY_WIDTH_OFFSET = int((MAX_WIDTH - MY_WIDTH) / 2)

    return (
        layer_index * (CASE_WIDTH + CASE_DISTANCE_X) + MY_WIDTH_OFFSET + MAIN_MARGIN_X,
        layer_num * (CASE_HEIGHT + CASE_DISTANCE_Y) + MAIN_MARGIN_Y,
    )


def svg_rect(left: int, top: int, width: int, height: int):
    return '<rect x="%d" y="%d" width="%d" height="%d" style="fill:pink;stroke-width:2;stroke:rgb(0,0,0)"/>' % (
        left, top, width, height
    )


print('<svg xmlns="http://www.w3.org/2000/svg" version="1.1">')

for layer_num in range(0, len(layer)):
    for layer_index in range(0, len(layer[layer_num])):
        x, y = case_address(layer_num, layer_index)
        print(' ', svg_rect(x, y, CASE_WIDTH, CASE_HEIGHT))


# for case in [(0, 0), (1, 0), (1, 1)]: # (layer_num, layer_index)
#     x, y = case_address(case[0], case[1])
#
#     print(' ', svg_rect(x, y, CASE_WIDTH, CASE_HEIGHT))
    # print(case_address(case[0], case[1]))

print('</svg>')
