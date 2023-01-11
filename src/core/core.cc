#include "bfs.h"
#include "core.h"
#include "raw_code.h"

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

#define RELEASE(NEXT_CODE, FILTER) { \
    cache_t next_case = { \
        .code = NEXT_CODE, \
        .mask = F_1x1 << next_addr, \
        .filter = FILTER, \
        .addr = next_addr \
    }; \
    cache_insert(next_case); \
}

#define release_1x1(FILTER) RELEASE(NEXT_CODE_1x1, FILTER)
#define release_1x2(FILTER) RELEASE(NEXT_CODE_1x2, FILTER)
#define release_2x1(FILTER) RELEASE(NEXT_CODE_2x1, FILTER)
#define release_2x2(FILTER) RELEASE(NEXT_CODE_2x2, FILTER)

#define NEXT_CODE_1x1 (code & ~(F_1x1 << addr) | (C_1x1 << next_addr))
#define NEXT_CODE_1x2 (code & ~(F_1x2 << addr) | (C_1x2 << next_addr))
#define NEXT_CODE_2x1 (code & ~(F_2x1 << addr) | (C_2x1 << next_addr))
#define NEXT_CODE_2x2 (code & ~(F_2x2 << addr) | (C_2x2 << next_addr))

inline void Core::cache_insert(Core::cache_t &next_case) { // try to insert into cache
    auto *p = cache;
    for (; p < cache + cache_size; ++p) {
        if (p->code == next_case.code) {
            return; // already exist -> insert failed
        }
    }
    *p = next_case;
    ++cache_size;
}

void Core::move_1x1(uint64_t code, int addr) { // try to move target 1x1 block
    BFS_INIT
    while (!BFS_STOP) { // bfs search process
        BFS_LOAD
        if (ALLOW_UP && TOP_LIMIT(4) && !(code >> MOVE_UP & F_1x1)) {
            release_1x1(UP) // 1x1 block move up
        }
        if (ALLOW_DOWN && BOTTOM_LIMIT(15) && !(code >> MOVE_DOWN & F_1x1)) {
            release_1x1(DOWN) // 1x1 block move down
        }
        if (ALLOW_LEFT && NOT_COLUMN_0 && !(code >> MOVE_LEFT & F_1x1)) {
            release_1x1(LEFT) // 1x1 block move left
        }
        if (ALLOW_RIGHT && NOT_COLUMN_3 && !(code >> MOVE_RIGHT & F_1x1)) {
            release_1x1(RIGHT) // 1x1 block move right
        }
    }
}

void Core::move_1x2(uint64_t code, int addr) { // try to move target 1x2 block
    BFS_INIT
    while (!BFS_STOP) { // bfs search process
        BFS_LOAD
        if (ALLOW_UP && TOP_LIMIT(4) && !(code >> MOVE_UP & F_1x2)) {
            release_1x2(UP) // 1x2 block move up
        }
        if (ALLOW_DOWN && BOTTOM_LIMIT(14) && !(code >> MOVE_DOWN & F_1x2)) {
            release_1x2(DOWN) // 1x2 block move down
        }
        if (ALLOW_LEFT && NOT_COLUMN_0 && !(code >> MOVE_LEFT & F_1x1)) {
            release_1x2(LEFT) // 1x2 block move left
        }
        if (ALLOW_RIGHT && NOT_COLUMN_2 && !(code >> MOVE_RIGHT & F_1x1_R)) {
            release_1x2(RIGHT) // 1x2 block move right
        }
    }
}

void Core::move_2x1(uint64_t code, int addr) { // try to move target 2x1 block
    BFS_INIT
    while (!BFS_STOP) { // bfs search process
        BFS_LOAD
        if (ALLOW_UP && TOP_LIMIT(4) && !(code >> MOVE_UP & F_1x1)) {
            release_2x1(UP) // 2x1 block move up
        }
        if (ALLOW_DOWN && BOTTOM_LIMIT(11) && !(code >> MOVE_DOWN & F_1x1_D)) {
            release_2x1(DOWN) // 2x1 block move down
        }
        if (ALLOW_LEFT && NOT_COLUMN_0 && !(code >> MOVE_LEFT & F_2x1)) {
            release_2x1(LEFT) // 2x1 block move left
        }
        if (ALLOW_RIGHT && NOT_COLUMN_3 && !(code >> MOVE_RIGHT & F_2x1)) {
            release_2x1(RIGHT) // 2x1 block move right
        }
    }
}

void Core::move_2x2(uint64_t code, int addr) { // try to move target 2x2 block
    BFS_INIT
    while (!BFS_STOP) { // bfs search process
        BFS_LOAD
        if (ALLOW_UP && TOP_LIMIT(4) && !(code >> MOVE_UP & F_1x2)) {
            release_2x2(UP) // 2x2 block move up
        }
        if (ALLOW_DOWN && BOTTOM_LIMIT(10) && !(code >> MOVE_DOWN & F_1x2_D)) {
            release_2x2(DOWN) // 2x2 block move down
        }
        if (ALLOW_LEFT && NOT_COLUMN_0 && !(code >> MOVE_LEFT & F_2x1)) {
            release_2x2(LEFT) // 2x2 block move left
        }
        if (ALLOW_RIGHT && NOT_COLUMN_2 && !(code >> MOVE_RIGHT & F_2x1_R)) {
            release_2x2(RIGHT) // 2x2 block move right
        }
    }
}

void Core::next_step(uint64_t raw_code, void (*release)(uint64_t, uint64_t)) { // search next step cases
    auto code = raw_code;
    for (int addr = 0; code; addr += 3, code >>= 3) { // traverse every 3-bits
        switch (code & 0b111) { // match low 3-bits
            case B_1x1:
                move_1x1(raw_code, addr); // try to move 1x1 block
                break;
            case B_1x2:
                move_1x2(raw_code, addr); // try to move 1x2 block
                break;
            case B_2x1:
                move_2x1(raw_code, addr); // try to move 2x1 block
                break;
            case B_2x2:
                move_2x2(raw_code, addr); // try to move 2x2 block
                break;
            default:
                continue; // B_space or B_fill
        }
        if (cache_size != 1) { // found one or more next cases
            for (int i = 1; i < cache_size; ++i) {

                // TODO: try to send multi-items data
                release(cache[i].code, cache[i].mask); // release next cases

            }
        }
    }
}
