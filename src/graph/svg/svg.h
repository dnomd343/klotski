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

    float opacity = 0;
    float line_opacity = 0;
    std::string color = "pink";
    std::string line_color = "blue";

    std::string dump() const;
};
