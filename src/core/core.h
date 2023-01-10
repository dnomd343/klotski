#pragma once

#include <cstdint>

#define UP    (-12) // -4 * 3-bits
#define LEFT   (-3) // +1 * 3-bits
#define DOWN  (+12) // +4 * 3-bits
#define RIGHT  (+3) // +1 * 3-bits

#define F_1x1 (uint64_t)0x7 // 111
#define F_1x2 (uint64_t)0x3F // 111 111
#define F_2x1 (uint64_t)0x7007 // 111 000 000 000 111
#define F_2x2 (uint64_t)0x3F03F // 111 111 000 000 111 111

#define F_1x1_R (uint64_t)0x38 // 111 000
#define F_1x1_D (uint64_t)0x7000 // 111 000 000 000 000
#define F_2x1_R (uint64_t)0x38038 // 111 000 000 000 111 000
#define F_1x2_D (uint64_t)0x3F000 // 111 111 000 000 000 000

class Core {
public:
//    void next_step(uint64_t raw_code);
    uint32_t next_step(uint64_t raw_code);

private:
    struct cache_t {
        uint64_t code; // case raw code
        uint64_t mask; // only 000 or 111
        int filter; // UP | DOWN | LEFT | RIGHT
        int addr; // (0 ~ 19) * 3
    };

    int cache_size;
    cache_t cache[16];

    void move_1x1(uint64_t code, int addr);
    void move_1x2(uint64_t code, int addr);
    void move_2x1(uint64_t code, int addr);
    void move_2x2(uint64_t code, int addr);
    inline void cache_insert(Core::cache_t &next_case);
};
