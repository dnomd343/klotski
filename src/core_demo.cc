#include <iostream>
#include "raw_code.h"
#include "core_demo.h"

cache_t cache[16];

// TODO: use cache_t *move_cache_top?
int cache_size;

inline bool cache_insert(uint64_t code) {

//    std::cout << "cache insert" << std::endl;

    auto *p = cache;
    for (; p < cache + cache_size; ++p) {
        if (*p == code) {
            return false; // already exist -> insert failed
        }
    }
    *p = code;
//    p->code = code;
//    p->filter = filter;
    ++cache_size;
    return true;
}

/// input -> raw_code / block address
void move_1x1(uint64_t code, int addr) {

//    std::cout << "enter move_1x1 at " << addr << std::endl;

    int target_addr;
    uint64_t ret_code;

    /// try to move up
    if (addr >= 4 * 3 && !(code >> (target_addr = addr + UP) & F_1x1)) {
        ret_code = code & ~(F_1x1 << addr) | C_1x1 << target_addr;
        if (cache_insert(ret_code)) {
            move_1x1(ret_code, target_addr);
        }
    }

    /// try to move down
    if (addr <= 15 * 3 && !(code >> (target_addr = addr + DOWN) & F_1x1)) {
        ret_code = code & ~(F_1x1 << addr) | C_1x1 << target_addr;
        if (cache_insert(ret_code)) {
            move_1x1(ret_code, target_addr);
        }
    }

    /// try to move left
    if ((addr & 0b11) != 0 && !(code >> (target_addr = addr + LEFT) & F_1x1)) {
        ret_code = code & ~(F_1x1 << addr) | C_1x1 << target_addr;
        if (cache_insert(ret_code)) {
            move_1x1(ret_code, target_addr);
        }
    }

    /// try to move right
    if ((addr & 0x3) != 1 && !(code >> (target_addr = addr + RIGHT) & F_1x1)) {
        ret_code = code & ~(F_1x1 << addr) | C_1x1 << target_addr;
        if (cache_insert(ret_code)) {
            move_1x1(ret_code, target_addr);
        }
    }
}

void next_step(uint64_t raw_code, uint64_t mask) {

//    std::cout << RawCode(raw_code).dump_case();
//    printf("mask -> %016lX\n", mask);

    // case 1 -> 8.8s

    for (int i = 0; i < 1000000000; ++i) {
        cache_size = 1;
        cache[0] = raw_code;
        move_1x1(raw_code, 17 * 3);
    }

//    std::cout << cache_size << std::endl;
//
//    std::cout << RawCode(cache[0]).dump_case() << std::endl;
//    std::cout << RawCode(cache[1]).dump_case() << std::endl;
//    std::cout << RawCode(cache[2]).dump_case() << std::endl;

}
