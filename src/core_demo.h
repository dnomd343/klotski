#pragma once

#include <cstdint>

#define UP      (-12) // -4 * 3-bits
#define LEFT     (-3) // +1 * 3-bits
#define DOWN    (+12) // +4 * 3-bits
#define DOWN_2  (+24) // +8 * 3-bits
#define RIGHT    (+3) // +1 * 3-bits
#define RIGHT_2  (+6) // +2 * 3-bits

struct cache_t {
    uint64_t code; // raw code
//    uint64_t mask; // only 000 or 111
//    int filter;
    int addr;
};

//typedef uint64_t cache_t;

void next_step(uint64_t raw_code, uint64_t mask);
