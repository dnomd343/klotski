#!/usr/bin/env python3
# -*- coding: utf-8 -*-

CASE_WIDTH = 120
CASE_HEIGHT = 180

CASE_DISTANCE_X = 200
CASE_DISTANCE_Y = 100

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

print(layer)
print(link)

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


for case in [(0, 0), (1, 0), (1, 1)]: # (layer_num, layer_index)
    print(case_address(case[0], case[1]))
