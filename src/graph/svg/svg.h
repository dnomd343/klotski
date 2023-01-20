#pragma once

#include <string>
#include <cstdint>

class SvgRect {
public:
    uint64_t top;
    uint64_t left;
    uint64_t width;
    uint64_t height;

    uint64_t radius = 0;
    uint32_t stroke = 1;

    std::string color;
    std::string line_color;

    float opacity = 0;
    float line_opacity = 0;

    std::string dump() const;
};
