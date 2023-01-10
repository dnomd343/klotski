#include <iostream>
#include "raw_code.h"
#include "core_demo.h"

cache_t cache[16];

// TODO: use cache_t *move_cache_top?
int cache_size;

inline bool cache_insert(cache_t &new_item) {

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

void move_1x1_demo(uint64_t code, int addr) {

    cache_size = 1;
    cache[0].code = code;
    cache[0].addr = addr;

    int count = 0;

    while (count != cache_size) {

        code = cache[count].code;
        addr = cache[count].addr;
        ++count;

        uint64_t ret_code;

        int next_addr;

        /// try to move up
        if (addr >= 4 * 3 && !(code >> (next_addr = addr + UP) & F_1x1)) {
            ret_code = code & ~(F_1x1 << addr) | (C_1x1 << next_addr);
            cache_t new_item = {ret_code, next_addr};
            cache_insert(new_item);

        }

        /// try to move down
        if (addr <= 15 * 3 && !(code >> (next_addr = addr + DOWN) & F_1x1)) {
            ret_code = code & ~(F_1x1 << addr) | (C_1x1 << next_addr);
            cache_t new_item = {ret_code, next_addr};
            cache_insert(new_item);

        }

        /// try to move left
        if ((addr & 0b11) != 0 && !(code >> (next_addr = addr + LEFT) & F_1x1)) {
            ret_code = code & ~(F_1x1 << addr) | (C_1x1 << next_addr);
            cache_t new_item = {ret_code, next_addr};
            cache_insert(new_item);

        }

        /// try to move right
        if ((addr & 0x3) != 1 && !(code >> (next_addr = addr + RIGHT) & F_1x1)) {
            ret_code = code & ~(F_1x1 << addr) | (C_1x1 << next_addr);
            cache_t new_item = {ret_code, next_addr};
            cache_insert(new_item);

        }

    }


}


void next_step(uint64_t raw_code, uint64_t mask) {

//    std::cout << RawCode(raw_code).dump_case();
//    printf("mask -> %016lX\n", mask);

    for (int i = 0; i < 1000000000; ++i) {
//        cache_size = 1;
//        cache[0].code = raw_code;
//        move_1x1(raw_code, 17 * 3);
        move_1x1_demo(raw_code, 17 * 3);
    }

    std::cout << cache_size << std::endl;
    std::cout << RawCode(cache[0].code).dump_case() << std::endl;
    std::cout << RawCode(cache[1].code).dump_case() << std::endl;
    std::cout << RawCode(cache[2].code).dump_case() << std::endl;

}
