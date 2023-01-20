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
    /// case options
    float CASE_RADIUS = 0.2;
    uint32_t CASE_BORDER_WIDTH = 2;
    std::string CASE_BG_COLOR = "pink";
    std::string CASE_BORDER_COLOR = "blue";

    /// block options
    float BLOCK_RADIUS = 0.3;
    uint32_t BLOCK_BORDER_WIDTH = 1;
    std::string BLOCK_BG_COLOR = "green";
    std::string BLOCK_BORDER_COLOR = "yellow";

    // TODO: opacity options

};

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
