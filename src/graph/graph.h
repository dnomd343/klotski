#pragma once

#include "analyse.h"
#include "svg/svg.h"
#include "raw_code.h"

class CaseSkin {
public:
    /// case options
    float CASE_RADIUS = 0.2;
    float CASE_BORDER_WIDTH = 1.5;
    std::string CASE_BG_COLOR = "#52575c";
    std::string CASE_BORDER_COLOR = "#292a26";

    /// block options
    float BLOCK_RADIUS = 0.3;
    float BLOCK_BORDER_WIDTH = 0.8;
    std::string BLOCK_BG_COLOR = "#C6840C";
    std::string BLOCK_BORDER_COLOR = "#f8f8f8";

    // TODO: opacity options

};

class GraphCase {
public:
    Point start;
    uint64_t code;
    uint32_t block_gap;
    uint32_t block_width;

    void render(SvgGraph &svg, const CaseSkin &skin) const;
    GraphCase(Point p, uint64_t c, uint32_t gap, uint32_t width) : start(p), code(c) {
        block_gap = gap;
        block_width = width;
    }
};

class Graph {

public:

    uint64_t BLOCK_LENGTH = 10;
    uint64_t BLOCK_GAP = 2;

    uint64_t CASE_GAP_X = 16;
    uint64_t CASE_GAP_Y = 32;

    uint64_t CASE_WIDTH = BLOCK_LENGTH * 4 + BLOCK_GAP * 5;
    uint64_t CASE_HEIGHT = BLOCK_LENGTH * 5 + BLOCK_GAP * 6;

    uint64_t LINE_OFFSET = 3;

    void svg_demo(Analyse::track_data_t track_data);

};
