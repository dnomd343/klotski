#pragma once

#include "analyse.h"



class Graph {

public:

    uint64_t BLOCK_LENGTH = 12;
    uint64_t BLOCK_GAP = 4;

    uint64_t CASE_GAP_X = 12;
    uint64_t CASE_GAP_Y = 20;

    uint64_t CASE_WIDTH = BLOCK_LENGTH * 4 + BLOCK_GAP * 5;
    uint64_t CASE_HEIGHT = BLOCK_LENGTH * 5 + BLOCK_GAP * 6;

    void svg_demo(Analyse::track_data_t track_data);

};
