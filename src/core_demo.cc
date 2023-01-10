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

//inline void demo(uint64_t code, int addr, int next_addr, int f) {
//    cache_t next_case = {
//        .code = code & ~(F_1x1 << addr) | (C_1x1 << next_addr),
//        .mask = (uint64_t)0b111 << next_addr,
//        .filter = f,
//        .addr = next_addr
//    };
//    cache_insert(next_case);
//}

void move_1x1_demo(uint64_t code, int addr) {

    cache_size = 1;
    cache[0].code = code;
    cache[0].addr = addr;
    cache[0].filter = 0; // filter unset

    int count = 0;

    int next_addr;

//    auto demo = [&](int f) {
//        cache_t next_case = {
//            .code = code & ~(F_1x1 << addr) | (C_1x1 << next_addr),
//            .mask = (uint64_t)0b111 << next_addr,
//            .filter = f,
//            .addr = next_addr
//        };
//        cache_insert(next_case);
//    };

    while (count != cache_size) {

        code = cache[count].code;
        addr = cache[count].addr;
        int filter = cache[count].filter;

        ++count;


        /// try to move up
        if (filter != UP && addr >= 4 * 3 && !(code >> (next_addr = addr + UP) & F_1x1)) {
            cache_t next_case = {
                .code = code & ~(F_1x1 << addr) | (C_1x1 << next_addr),
                .mask = (uint64_t)0b111 << next_addr,
                .filter = DOWN,
                .addr = next_addr
            };
            cache_insert(next_case);
//            demo(DOWN);
//            demo(code, addr, next_addr, DOWN);
        }

        /// try to move down
        if (filter != DOWN && addr <= 15 * 3 && !(code >> (next_addr = addr + DOWN) & F_1x1)) {
            cache_t next_case = {
                .code = code & ~(F_1x1 << addr) | (C_1x1 << next_addr),
                .mask = (uint64_t)0b111 << next_addr,
                .filter = UP,
                .addr = next_addr
            };
            cache_insert(next_case);
//            demo(UP);
//            demo(code, addr, next_addr, UP);
        }

        /// try to move left
        if (filter != LEFT && (addr & 3) != 0 && !(code >> (next_addr = addr + LEFT) & F_1x1)) {
            cache_t next_case = {
                .code = code & ~(F_1x1 << addr) | (C_1x1 << next_addr),
                .mask = (uint64_t)0b111 << next_addr,
                .filter = RIGHT,
                .addr = next_addr
            };
            cache_insert(next_case);
//            demo(RIGHT);
//            demo(code, addr, next_addr, RIGHT);
        }

        /// try to move right
        if (filter != RIGHT && (addr & 3) != 1 && !(code >> (next_addr = addr + RIGHT) & F_1x1)) {
            cache_t next_case = {
                .code = code & ~(F_1x1 << addr) | (C_1x1 << next_addr),
                .mask = (uint64_t)0b111 << next_addr,
                .filter = LEFT,
                .addr = next_addr
            };
            cache_insert(next_case);
//            demo(LEFT);
//            demo(code, addr, next_addr, LEFT);
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
