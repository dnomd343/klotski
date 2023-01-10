#include <iostream>
#include "raw_code.h"
#include "core_demo.h"

int cache_size;
cache_t cache[16];

#define ALLOW_UP    (filter != UP)
#define ALLOW_DOWN  (filter != DOWN)
#define ALLOW_LEFT  (filter != LEFT)
#define ALLOW_RIGHT (filter != RIGHT)

#define MOVE_UP    (next_addr = addr + UP)
#define MOVE_DOWN  (next_addr = addr + DOWN)
#define MOVE_LEFT  (next_addr = addr + LEFT)
#define MOVE_RIGHT (next_addr = addr + RIGHT)

#define release_1x1(_filter) { \
    cache_t next_case = { \
        .code = code & ~(F_1x1 << addr) | (C_1x1 << next_addr), \
        .mask = F_1x1 << next_addr, \
        .filter = _filter, \
        .addr = next_addr \
    }; \
    cache_insert(next_case); \
}

#define release_1x2(_filter) { \
    cache_t next_case = { \
        .code = code & ~(F_1x2 << addr) | (C_1x2 << next_addr), \
        .mask = F_1x1 << next_addr, \
        .filter = _filter, \
        .addr = next_addr \
    }; \
    cache_insert(next_case); \
}

#define release_2x1(_filter) { \
    cache_t next_case = { \
        .code = code & ~(F_2x1 << addr) | (C_2x1 << next_addr), \
        .mask = F_1x1 << next_addr, \
        .filter = _filter, \
        .addr = next_addr \
    }; \
    cache_insert(next_case); \
}

#define release_2x2(_filter) { \
    cache_t next_case = { \
        .code = code & ~(F_2x2 << addr) | (C_2x2 << next_addr), \
        .mask = F_1x1 << next_addr, \
        .filter = _filter, \
        .addr = next_addr \
    }; \
    cache_insert(next_case); \
}

inline bool cache_insert(cache_t &new_item) {

//    static int insert_num = 0;
//    std::cout << "insert times: " << ++insert_num << std::endl;

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
    cache[0].code = code;
    cache[0].addr = addr;
    cache[0].filter = 0; // filter unset

    int current = 0;
    while (current != cache_size) { // start bfs search
        code = cache[current].code;
        addr = cache[current].addr;
        int next_addr; // address after block moved
        int filter = cache[current++].filter; // case filter

        if (ALLOW_UP && addr >= 4 * 3 && !(code >> MOVE_UP & F_1x1)) {
            release_1x1(-UP) // 1x1 block move up
        }
        if (ALLOW_DOWN && addr <= 15 * 3 && !(code >> MOVE_DOWN & F_1x1)) {
            release_1x1(-DOWN) // 1x1 block move down
        }
        if (ALLOW_LEFT && (addr & 3) != 0 && !(code >> MOVE_LEFT & F_1x1)) {
            release_1x1(-LEFT) // 1x1 block move left
        }
        if (ALLOW_RIGHT && (addr & 3) != 1 && !(code >> MOVE_RIGHT & F_1x1)) {
            release_1x1(-RIGHT) // 1x1 block move right
        }
    }
}

void move_1x2(uint64_t code, int addr) {
    cache_size = 1;
    cache[0].code = code;
    cache[0].addr = addr;
    cache[0].filter = 0; // filter unset

    int current = 0;
    while (current != cache_size) { // start bfs search
        code = cache[current].code;
        addr = cache[current].addr;
        int next_addr; // address after block moved
        int filter = cache[current++].filter; // case filter

        if (ALLOW_UP && addr >= 4 * 3 && !(code >> MOVE_UP & F_1x2)) {
            release_1x2(-UP) // 1x2 block move up
        }
        if (ALLOW_DOWN && addr <= 14 * 3 && !(code >> MOVE_DOWN & F_1x2)) {
            release_1x2(-DOWN) // 1x2 block move down
        }
        if (ALLOW_LEFT && (addr & 3) != 0 && !(code >> MOVE_LEFT & F_1x1)) {
            release_1x2(-LEFT) // 1x2 block move left
        }
        if (ALLOW_RIGHT && (addr & 3) != 2 && !(code >> MOVE_RIGHT & F_1x1_R)) {
            release_1x2(-RIGHT) // 1x2 block move right
        }
    }
}

void move_2x1(uint64_t code, int addr) {
    cache_size = 1;
    cache[0].code = code;
    cache[0].addr = addr;
    cache[0].filter = 0; // filter unset

    int current = 0;
    while (current != cache_size) { // start bfs search
        code = cache[current].code;
        addr = cache[current].addr;
        int next_addr; // address after block moved
        int filter = cache[current++].filter; // case filter

        if (ALLOW_UP && addr >= 4 * 3 && !(code >> MOVE_UP & F_1x1)) {
            release_2x1(-UP) // 2x1 block move up
        }
        if (ALLOW_DOWN && addr <= 11 * 3 && !(code >> MOVE_DOWN & F_1x1_D)) {
            release_2x1(-DOWN) // 2x1 block move down
        }
        if (ALLOW_LEFT && (addr & 0x3) != 0 && !(code >> MOVE_LEFT & F_2x1)) {
            release_2x1(-LEFT) // 2x1 block move left
        }
        if (ALLOW_RIGHT && (addr & 0x3) != 1 && !(code >> MOVE_RIGHT & F_2x1)) {
            release_2x1(-RIGHT) // 2x1 block move right
        }
    }
}


void move_2x2(uint64_t code, int addr) {
    cache_size = 1;
    cache[0].code = code;
    cache[0].addr = addr;
    cache[0].filter = 0; // filter unset

    int current = 0;
    while (current != cache_size) { // start bfs search
        code = cache[current].code;
        addr = cache[current].addr;
        int next_addr; // address after block moved
        int filter = cache[current++].filter; // case filter

        if (ALLOW_UP && addr >= 4 * 3 && !(code >> MOVE_UP & F_1x2)) {
            release_2x2(-UP) // 2x2 block move up
        }
        if (ALLOW_DOWN && addr <= 10 * 3 && !(code >> MOVE_DOWN & F_1x2_D)) {
            release_2x2(-DOWN) // 2x2 block move down
        }
        if (ALLOW_LEFT && (addr & 3) != 0 && !(code >> MOVE_LEFT & F_2x1)) {
            release_2x2(-LEFT) // 2x2 block move left
        }
        if (ALLOW_RIGHT && (addr & 3) != 2 && !(code >> MOVE_RIGHT & F_2x1_R)) {
            release_2x2(-RIGHT) // 2x2 block move right
        }
    }
}


void next_step(uint64_t raw_code, uint64_t mask) {

    int addr = 17;
    raw_code = RawCode(CommonCode("4fea134")).unwrap();
//    move_1x1(raw_code, addr * 3);

//    int addr = 9;
//    raw_code = RawCode(CommonCode("1003")).unwrap();
//    move_1x1(raw_code, addr * 3);

//    int addr = 9;
//    raw_code = RawCode(CommonCode("1002")).unwrap();
//    move_2x1(raw_code, addr * 3);

//    int addr = 5;
//    raw_code = RawCode(CommonCode("5")).unwrap();
//    move_2x2(raw_code, addr * 3);


//    std::cout << RawCode(raw_code).dump_case();
//    printf("mask -> %016lX\n", mask);

    for (int i = 0; i < 1000000000; ++i) {
//    for (int i = 0; i < 50000000; ++i) {
        move_1x1(raw_code, addr * 3);
    }

    std::cout << "cache size: " << cache_size << std::endl;

    for (int i = 0; i < cache_size; ++i) {
        std::cout << "=======" << std::endl;

        std::cout << RawCode(cache[i].code).dump_case();

        if (i != 0) {
            auto _mask = cache[i].mask;
            std::cout << std::endl;
            for (int n = 0; n < 20; ++n, _mask >>= 3) {
                if (_mask & 0b111) {
                    std::cout << "+ ";
                } else {
                    std::cout << ". ";
                }
                if ((n & 0b11) == 0b11) {
                    std::cout << std::endl;
                }
            }
        }

        std::cout << "=======" << std::endl << std::endl;
    }

}
