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

cache_t cache[16];

/// why is it slower to use pointer?
//cache_t *cache_top;

int cache_size;

inline bool cache_insert(cache_t &new_item) {

//    auto *p = cache;
//    for (; p < cache_top; ++p) {
//        if (p->code == new_item.code) {
//            return false; // already exist -> insert failed
//        }
//    }
//
//    *p = new_item;
//    ++cache_top;
//    return true;


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
//    cache_top = cache + 1;
    cache[0].code = code; // load into queue
    cache[0].addr = addr;
    cache[0].filter = 0; // filter unset

    int count = 0;
//    cache_t *current = cache;

    while (count != cache_size) { // start bfs search
//    while (current != cache_top) { // start bfs search

        code = cache[count].code;
        addr = cache[count].addr;
//        code = current->code;
//        addr = current->addr;
        int next_addr; // address after block moved
        int filter = cache[count++].filter; // case filter
//        int filter = current->filter; // case filter
//        ++current;

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
//    std::cout << (cache_top - cache) << std::endl;
    std::cout << RawCode(cache[0].code).dump_case() << std::endl;
    std::cout << RawCode(cache[1].code).dump_case() << std::endl;
    std::cout << RawCode(cache[2].code).dump_case() << std::endl;

}
