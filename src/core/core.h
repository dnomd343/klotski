#pragma once

#include <cstdint>

#define UP    (-12) // -4 * 3-bits
#define LEFT   (-3) // +1 * 3-bits
#define DOWN  (+12) // +4 * 3-bits
#define RIGHT  (+3) // +1 * 3-bits

class Core {
public:
    void next_step(uint64_t code, uint64_t mask);

    Core(void (*release_func)(uint64_t code, uint64_t mask)) : release(release_func) {}

private:
    struct cache_t {
        uint64_t code; // case raw code
        uint64_t mask; // only 000 or 111
        int filter; // UP | DOWN | LEFT | RIGHT
        int addr; // (0 ~ 19) * 3
    };

    int cache_size;
    cache_t cache[16];
    void (*release)(uint64_t code, uint64_t mask);

    void move_1x1(uint64_t code, int addr);
    void move_1x2(uint64_t code, int addr);
    void move_2x1(uint64_t code, int addr);
    void move_2x2(uint64_t code, int addr);
    inline void cache_insert(Core::cache_t &next_case);
};
