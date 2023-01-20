#pragma once

#include "analyse.h"
#include "svg/svg.h"

#include "raw_code.h"

//struct BlockOpt {
//    uint32_t gap;
//    uint32_t width;
//};

class CaseSkin {
public:
    std::string color = "pink";
};

//class SvgCase {
//public:
//
//    uint64_t code;
//
//    uint64_t top = 0;
//    uint64_t left = 0;
//
//    uint32_t gap = 4;
//    uint32_t width = 12;
//
//    void render() const;
//
//};

class GraphCase {
public:
    Point start;
    RawCode code;
    uint32_t block_gap;
    uint32_t block_width;

    GraphCase(Point p, RawCode c, uint32_t gap, uint32_t width) : start(p), code(c) {
        block_gap = gap;
        block_width = width;
    }

    void render(SvgGraph &svg, const CaseSkin &skin) const;

};

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
