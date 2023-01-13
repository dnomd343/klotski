#!/usr/bin/env python3
# -*- coding: utf-8 -*-

CASE_WIDTH = 48
CASE_HEIGHT = 72

CASE_DISTANCE_X = 80
CASE_DISTANCE_Y = 40

MAIN_MARGIN_X = 10
MAIN_MARGIN_Y = 8

LINK_OFFSET = 1

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


def link_address(start_layer_num: int, start_layer_index: int, end_layer_num: int, end_layer_index: int) -> (int, int, int, int):

    x1, y1 = case_address(start_layer_num, start_layer_index)
    x2, y2 = case_address(end_layer_num, end_layer_index)

    return (
        x1 + int(CASE_WIDTH / 2),
        y1 + CASE_HEIGHT + LINK_OFFSET,
        x2 + int(CASE_WIDTH / 2),
        y2 - LINK_OFFSET,
    )


def svg_rect(left: int, top: int, width: int, height: int):
    return '<rect x="%d" y="%d" width="%d" height="%d" style="fill:pink;stroke-width:2;stroke:rgb(0,0,0)"/>' % (
        left, top, width, height
    )


def svg_line(start_x: int, start_y: int, end_x: int, end_y: int):
    return '<line x1="%d" y1="%d" x2="%d" y2="%d" style="stroke:black;stroke-width:1"/>' % (
        start_x, start_y, end_x, end_y
    )

MAIN_WIDTH = MAX_CASE_X * CASE_WIDTH + (MAX_CASE_X - 1) * CASE_DISTANCE_X + MAIN_MARGIN_X * 2
MAIN_HEIGHT = MAX_CASE_Y * CASE_HEIGHT + (MAX_CASE_Y - 1) * CASE_DISTANCE_Y + MAIN_MARGIN_Y * 2

print('<svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="%d" height="%d">' % (MAIN_WIDTH, MAIN_HEIGHT))

for layer_num in range(0, len(layer)):
    for layer_index in range(0, len(layer[layer_num])):
        x, y = case_address(layer_num, layer_index)
        print(' ', svg_rect(x, y, CASE_WIDTH, CASE_HEIGHT))

for layer_num in range(0, len(link)):
    for layer_index in range(0, len(link[layer_num])):
        for next_index in range(0, len(link[layer_num][layer_index])):

            # print('(%d, %d) -> (%d, %d)' % (layer_num, layer_index, layer_num + 1, next_index))
            x1, y1, x2, y2 = link_address(layer_num, layer_index, layer_num + 1, next_index)
            print(' ', svg_line(x1, y1, x2, y2))

# x1, y1, x2, y2 = link_address(0, 0, 1, 0)
# print(' ', svg_line(x1, y1, x2, y2))

print('</svg>')
