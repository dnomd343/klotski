#include <iostream>
#include "raw_code.h"
#include "core_demo.h"

#define release_1x1(filter_dir) {\
    cache_t next_case = { \
        .code = code & ~(F_1x1 << addr) | (C_1x1 << next_addr), \
        .mask = (uint64_t)0b111 << next_addr, \
        .filter = filter_dir, \
        .addr = next_addr \
    }; \
    cache_insert(next_case); \
}

int cache_size;
cache_t cache[16];

inline bool cache_insert(cache_t &new_item) {

    // TODO: try closed hashing
//    auto volatile hash = new_item.code;
//    printf("%016lX -> ", hash); // 64-bits
//    hash ^= hash >> 32;
//    printf("%08lX -> ", hash & 0xFFFFFFFF); // 32-bits
//    hash ^= hash >> 16;
//    printf("%04lX -> ", hash & 0xFFFF); // 16-bits
//    hash ^= hash >> 8;
//    printf("%02lX -> ", hash & 0xFF); // 8-bits
//    hash ^= hash >> 4;
//    printf("%01lX\n", hash & 0xF); // 4-bits

    auto *p = cache;
    for (; p < cache + cache_size; ++p) {
        if (p->code == new_item.code) {
            return false; // already exist -> insert failed
        }
    }
    *p = new_item;
    ++cache_size;
    return true;
}

void move_1x1(uint64_t code, int addr) {
    cache_size = 1;
    cache[0].code = code; // load into queue
    cache[0].addr = addr;
    cache[0].filter = 0; // filter unset

    int current = 0;

    while (current != cache_size) { // start bfs search
        code = cache[current].code;
        addr = cache[current].addr;
        int next_addr; // address after block moved
        int filter = cache[current++].filter; // case filter

        if (filter != UP && addr >= 4 * 3 && !(code >> (next_addr = addr + UP) & F_1x1)) {
            release_1x1(-UP); // block can move up
        }
        if (filter != DOWN && addr <= 15 * 3 && !(code >> (next_addr = addr + DOWN) & F_1x1)) {
            release_1x1(-DOWN); // block can move down
        }
        if (filter != LEFT && (addr & 3) != 0 && !(code >> (next_addr = addr + LEFT) & F_1x1)) {
            release_1x1(-LEFT); // block can move left
        }
        if (filter != RIGHT && (addr & 3) != 1 && !(code >> (next_addr = addr + RIGHT) & F_1x1)) {
            release_1x1(-RIGHT); // block can move right
        }
    }
}


void next_step(uint64_t raw_code, uint64_t mask) {

//    std::cout << RawCode(raw_code).dump_case();
//    printf("mask -> %016lX\n", mask);

    for (int i = 0; i < 1000000000; ++i) {
        move_1x1(raw_code, 17 * 3);
    }

    std::cout << cache_size << std::endl;
    std::cout << RawCode(cache[0].code).dump_case() << std::endl;
    std::cout << RawCode(cache[1].code).dump_case() << std::endl;
    std::cout << RawCode(cache[2].code).dump_case() << std::endl;

}
