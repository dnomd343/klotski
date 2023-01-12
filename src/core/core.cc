#include "core.h"
#include "common.h"

#define ALLOW_UP    (filter != -UP)
#define ALLOW_DOWN  (filter != -DOWN)
#define ALLOW_LEFT  (filter != -LEFT)
#define ALLOW_RIGHT (filter != -RIGHT)

#define NOT_COLUMN_0 ((addr & 3) != 0)
#define NOT_COLUMN_2 ((addr & 3) != 2)
#define NOT_COLUMN_3 ((addr & 3) != 1)

#define MOVE_UP    (next_addr = addr + UP)
#define MOVE_DOWN  (next_addr = addr + DOWN)
#define MOVE_LEFT  (next_addr = addr + LEFT)
#define MOVE_RIGHT (next_addr = addr + RIGHT)

#define TOP_LIMIT(ADDR)    (addr >= ADDR * 3)
#define BOTTOM_LIMIT(ADDR) (addr <= ADDR * 3)

#define CHECK_UP(MASK)    !(code >> MOVE_UP & MASK)
#define CHECK_DOWN(MASK)  !(code >> MOVE_DOWN & MASK)
#define CHECK_LEFT(MASK)  !(code >> MOVE_LEFT & MASK)
#define CHECK_RIGHT(MASK) !(code >> MOVE_RIGHT & MASK)

#define RELEASE_1x1(FILTER) RELEASE(NEXT_CODE_1x1, FILTER)
#define RELEASE_1x2(FILTER) RELEASE(NEXT_CODE_1x2, FILTER)
#define RELEASE_2x1(FILTER) RELEASE(NEXT_CODE_2x1, FILTER)
#define RELEASE_2x2(FILTER) RELEASE(NEXT_CODE_2x2, FILTER)

#define NEXT_CODE_1x1 (code & ~(F_1x1 << addr) | (C_1x1 << next_addr))
#define NEXT_CODE_1x2 (code & ~(F_1x2 << addr) | (C_1x2 << next_addr))
#define NEXT_CODE_2x1 (code & ~(F_2x1 << addr) | (C_2x1 << next_addr))
#define NEXT_CODE_2x2 (code & ~(F_2x2 << addr) | (C_2x2 << next_addr))

////////////////////////////////////////

#define BFS_INIT \
int next_addr; \
int current = 0; \
cache[0].addr = addr;

#define BFS_LOAD \
code = cache[current].code; \
addr = cache[current].addr; \
int filter = cache[current++].filter;

#define BFS_STOP (current == cache_size)

////////////////////////////////////////

#define RELEASE(NEXT_CODE, FILTER) \
cache_t next_case = { \
    .code = NEXT_CODE, \
    .mask = F_1x1 << next_addr, \
    .filter = FILTER, \
    .addr = next_addr \
}; \
cache_insert(next_case);

////////////////////////////////////////

inline void Core::cache_insert(Core::cache_t &next_case) { // try to insert into cache
    auto *cache_ptr = cache;
    for (; cache_ptr < cache + cache_size; ++cache_ptr) {
        if (cache_ptr->code == next_case.code) {
            return; // already exist -> insert failed
        }
    }
    *cache_ptr = next_case; // cache push back
    ++cache_size;
}

void Core::move_1x1(uint64_t code, int addr) { // try to move target 1x1 block
    BFS_INIT
    while (!BFS_STOP) { // bfs search process
        BFS_LOAD
        if (ALLOW_UP && TOP_LIMIT(4) && CHECK_UP(F_1x1)) {
            RELEASE_1x1(UP) // 1x1 block move up
        }
        if (ALLOW_DOWN && BOTTOM_LIMIT(15) && CHECK_DOWN(F_1x1)) {
            RELEASE_1x1(DOWN) // 1x1 block move down
        }
        if (ALLOW_LEFT && NOT_COLUMN_0 && CHECK_LEFT(F_1x1)) {
            RELEASE_1x1(LEFT) // 1x1 block move left
        }
        if (ALLOW_RIGHT && NOT_COLUMN_3 && CHECK_RIGHT(F_1x1)) {
            RELEASE_1x1(RIGHT) // 1x1 block move right
        }
    }
}

void Core::move_1x2(uint64_t code, int addr) { // try to move target 1x2 block
    BFS_INIT
    while (!BFS_STOP) { // bfs search process
        BFS_LOAD
        if (ALLOW_UP && TOP_LIMIT(4) && CHECK_UP(F_1x2)) {
            RELEASE_1x2(UP) // 1x2 block move up
        }
        if (ALLOW_DOWN && BOTTOM_LIMIT(14) && CHECK_DOWN(F_1x2)) {
            RELEASE_1x2(DOWN) // 1x2 block move down
        }
        if (ALLOW_LEFT && NOT_COLUMN_0 && CHECK_LEFT(F_1x1)) {
            RELEASE_1x2(LEFT) // 1x2 block move left
        }
        if (ALLOW_RIGHT && NOT_COLUMN_2 && CHECK_RIGHT(F_1x1_R)) {
            RELEASE_1x2(RIGHT) // 1x2 block move right
        }
    }
}

void Core::move_2x1(uint64_t code, int addr) { // try to move target 2x1 block
    BFS_INIT
    while (!BFS_STOP) { // bfs search process
        BFS_LOAD
        if (ALLOW_UP && TOP_LIMIT(4) && CHECK_UP(F_1x1)) {
            RELEASE_2x1(UP) // 2x1 block move up
        }
        if (ALLOW_DOWN && BOTTOM_LIMIT(11) && CHECK_DOWN(F_1x1_D)) {
            RELEASE_2x1(DOWN) // 2x1 block move down
        }
        if (ALLOW_LEFT && NOT_COLUMN_0 && CHECK_LEFT(F_2x1)) {
            RELEASE_2x1(LEFT) // 2x1 block move left
        }
        if (ALLOW_RIGHT && NOT_COLUMN_3 && CHECK_RIGHT(F_2x1)) {
            RELEASE_2x1(RIGHT) // 2x1 block move right
        }
    }
}

void Core::move_2x2(uint64_t code, int addr) { // try to move target 2x2 block
    BFS_INIT
    while (!BFS_STOP) { // bfs search process
        BFS_LOAD
        if (ALLOW_UP && TOP_LIMIT(4) && CHECK_UP(F_1x2)) {
            RELEASE_2x2(UP) // 2x2 block move up
        }
        if (ALLOW_DOWN && BOTTOM_LIMIT(10) && CHECK_DOWN(F_1x2_D)) {
            RELEASE_2x2(DOWN) // 2x2 block move down
        }
        if (ALLOW_LEFT && NOT_COLUMN_0 && CHECK_LEFT(F_2x1)) {
            RELEASE_2x2(LEFT) // 2x2 block move left
        }
        if (ALLOW_RIGHT && NOT_COLUMN_2 && CHECK_RIGHT(F_2x1_R)) {
            RELEASE_2x2(RIGHT) // 2x2 block move right
        }
    }
}

void Core::next_step(uint64_t code, uint64_t mask) { // search next step cases
    cache[0].filter = 0; // without filter
    cache[0].code = code; // bfs root code
    auto range = code | mask;

    for (int addr = 0; range; addr += 3, range >>= 3) { // traverse every 3-bits
        switch (range & 0b111) { // match low 3-bits
            case B_1x1:
                move_1x1(code, addr); // try to move 1x1 block
                break;
            case B_1x2:
                move_1x2(code, addr); // try to move 1x2 block
                break;
            case B_2x1:
                move_2x1(code, addr); // try to move 2x1 block
                break;
            case B_2x2:
                move_2x2(code, addr); // try to move 2x2 block
                break;
            default:
                continue; // B_space or B_fill
        }
        if (cache_size != 1) { // found one or more next cases
            for (int i = 1; i < cache_size; ++i) {
                release(cache[i].code, cache[i].mask); // release next cases
            }
            cache_size = 1; // reset cache size
        }
    }
}
