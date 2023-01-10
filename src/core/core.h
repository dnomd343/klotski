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
    struct cache_t {
        uint64_t code; // raw code
        uint64_t mask; // only 000 or 111
        int filter;
        int addr;
    };

    int cache_size;
    cache_t cache[16];

    inline void cache_insert(Core::cache_t &next_case);

    void move_1x1(uint64_t code, int addr);
    void move_1x2(uint64_t code, int addr);
    void move_2x1(uint64_t code, int addr);
    void move_2x2(uint64_t code, int addr);

//    void move_1x1(int addr);
//    void move_1x2(int addr);
//    void move_2x1(int addr);
//    void move_2x2(int addr);

    void next_step(uint64_t raw_code);

//    void next_step();

//    uint64_t _code;

//    Core(uint64_t raw_code) : _code(raw_code) {}

};


