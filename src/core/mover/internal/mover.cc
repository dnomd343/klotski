#include "mover/mover.h"
#include "utils/common.h"

/// block move direction
#define DIR_UP    (-4 * 3)
#define DIR_LEFT  (-1 * 3)
#define DIR_DOWN  (+4 * 3)
#define DIR_RIGHT (+1 * 3)

/// block direction limit
#define ALLOW_UP    (filter != -DIR_UP)
#define ALLOW_DOWN  (filter != -DIR_DOWN)
#define ALLOW_LEFT  (filter != -DIR_LEFT)
#define ALLOW_RIGHT (filter != -DIR_RIGHT)

/// horizontal restraints
#define NOT_COLUMN_0 ((addr & 3) != 0b00)
#define NOT_COLUMN_2 ((addr & 3) != 0b10)
#define NOT_COLUMN_3 ((addr & 3) != 0b01)

/// try to move block
#define MOVE_UP    (next_addr = addr + DIR_UP)
#define MOVE_DOWN  (next_addr = addr + DIR_DOWN)
#define MOVE_LEFT  (next_addr = addr + DIR_LEFT)
#define MOVE_RIGHT (next_addr = addr + DIR_RIGHT)

/// vertical restraints
#define TOP_LIMIT(ADDR)    (addr >= ADDR * 3)
#define BOTTOM_LIMIT(ADDR) (addr <= ADDR * 3)

/// check if the block can move
#define CHECK_UP(MASK)    !(code >> MOVE_UP & MASK)
#define CHECK_DOWN(MASK)  !(code >> MOVE_DOWN & MASK)
#define CHECK_LEFT(MASK)  !(code >> MOVE_LEFT & MASK)
#define CHECK_RIGHT(MASK) !(code >> MOVE_RIGHT & MASK)

/// release next code
#define RELEASE_1x1(FILTER) RELEASE(NEXT_CODE_1x1, FILTER)
#define RELEASE_1x2(FILTER) RELEASE(NEXT_CODE_1x2, FILTER)
#define RELEASE_2x1(FILTER) RELEASE(NEXT_CODE_2x1, FILTER)
#define RELEASE_2x2(FILTER) RELEASE(NEXT_CODE_2x2, FILTER)

/// calculate next code
#define NEXT_CODE_1x1 ((code & ~(K_MASK_1x1_ << addr)) | (K_MASK_1x1 << next_addr))
#define NEXT_CODE_1x2 ((code & ~(K_MASK_1x2_ << addr)) | (K_MASK_1x2 << next_addr))
#define NEXT_CODE_2x1 ((code & ~(K_MASK_2x1_ << addr)) | (K_MASK_2x1 << next_addr))
#define NEXT_CODE_2x2 ((code & ~(K_MASK_2x2_ << addr)) | (K_MASK_2x2 << next_addr))

///////////////////////////////////////////////

#define RELEASE(NEXT_CODE, FILTER)  \
    cache_insert(cache_t {          \
        .code = NEXT_CODE,          \
        .mask = K_MASK_1x1_ << next_addr, \
        .filter = FILTER,           \
        .addr = next_addr           \
    });

///////////////////////////////////////////////

#define BFS_INIT     \
    int next_addr;   \
    int current = 0; \
    cache_[0].addr = addr;

#define BFS_LOAD                 \
    code = cache_[current].code; \
    addr = cache_[current].addr; \
    int filter = cache_[current++].filter;

#define BFS_STOP \
    (current == cache_size_)

///////////////////////////////////////////////

using klotski::codec::RawCode;
using klotski::mover::MaskMover;

inline void MaskMover::cache_insert(cache_t next_case) { // try to insert into cache
    auto *cache_ptr = cache_;
    for (; cache_ptr < cache_ + cache_size_; ++cache_ptr) {
        if (cache_ptr->code == next_case.code) {
            return; // already exist -> insert failed
        }
    }
    *cache_ptr = next_case; // cache push back
    ++cache_size_;
}

void MaskMover::move_1x1(uint64_t code, int addr) { // try to move target 1x1 block
    BFS_INIT
    while (!BFS_STOP) { // bfs search process
        BFS_LOAD
        if (ALLOW_UP && TOP_LIMIT(4) && CHECK_UP(K_MASK_1x1_)) {
            RELEASE_1x1(DIR_UP) // 1x1 block move up
        }
        if (ALLOW_DOWN && BOTTOM_LIMIT(15) && CHECK_DOWN(K_MASK_1x1_)) {
            RELEASE_1x1(DIR_DOWN) // 1x1 block move down
        }
        if (ALLOW_LEFT && NOT_COLUMN_0 && CHECK_LEFT(K_MASK_1x1_)) {
            RELEASE_1x1(DIR_LEFT) // 1x1 block move left
        }
        if (ALLOW_RIGHT && NOT_COLUMN_3 && CHECK_RIGHT(K_MASK_1x1_)) {
            RELEASE_1x1(DIR_RIGHT) // 1x1 block move right
        }
    }
}

void MaskMover::move_1x2(uint64_t code, int addr) { // try to move target 1x2 block
    BFS_INIT
    while (!BFS_STOP) { // bfs search process
        BFS_LOAD
        if (ALLOW_UP && TOP_LIMIT(4) && CHECK_UP(K_MASK_1x2_)) {
            RELEASE_1x2(DIR_UP) // 1x2 block move up
        }
        if (ALLOW_DOWN && BOTTOM_LIMIT(14) && CHECK_DOWN(K_MASK_1x2_)) {
            RELEASE_1x2(DIR_DOWN) // 1x2 block move down
        }
        if (ALLOW_LEFT && NOT_COLUMN_0 && CHECK_LEFT(K_MASK_1x1_)) {
            RELEASE_1x2(DIR_LEFT) // 1x2 block move left
        }
        if (ALLOW_RIGHT && NOT_COLUMN_2 && CHECK_RIGHT(K_MASK_1x1_R)) {
            RELEASE_1x2(DIR_RIGHT) // 1x2 block move right
        }
    }
}

void MaskMover::move_2x1(uint64_t code, int addr) { // try to move target 2x1 block
    BFS_INIT
    while (!BFS_STOP) { // bfs search process
        BFS_LOAD
        if (ALLOW_UP && TOP_LIMIT(4) && CHECK_UP(K_MASK_1x1_)) {
            RELEASE_2x1(DIR_UP) // 2x1 block move up
        }
        if (ALLOW_DOWN && BOTTOM_LIMIT(11) && CHECK_DOWN(K_MASK_1x1_D)) {
            RELEASE_2x1(DIR_DOWN) // 2x1 block move down
        }
        if (ALLOW_LEFT && NOT_COLUMN_0 && CHECK_LEFT(K_MASK_2x1_)) {
            RELEASE_2x1(DIR_LEFT) // 2x1 block move left
        }
        if (ALLOW_RIGHT && NOT_COLUMN_3 && CHECK_RIGHT(K_MASK_2x1_)) {
            RELEASE_2x1(DIR_RIGHT) // 2x1 block move right
        }
    }
}

void MaskMover::move_2x2(uint64_t code, int addr) { // try to move target 2x2 block
    BFS_INIT
    while (!BFS_STOP) { // bfs search process
        BFS_LOAD
        if (ALLOW_UP && TOP_LIMIT(4) && CHECK_UP(K_MASK_1x2_)) {
            RELEASE_2x2(DIR_UP) // 2x2 block move up
        }
        if (ALLOW_DOWN && BOTTOM_LIMIT(10) && CHECK_DOWN(K_MASK_1x2_D)) {
            RELEASE_2x2(DIR_DOWN) // 2x2 block move down
        }
        if (ALLOW_LEFT && NOT_COLUMN_0 && CHECK_LEFT(K_MASK_2x1_)) {
            RELEASE_2x2(DIR_LEFT) // 2x2 block move left
        }
        if (ALLOW_RIGHT && NOT_COLUMN_2 && CHECK_RIGHT(K_MASK_2x1_R)) {
            RELEASE_2x2(DIR_RIGHT) // 2x2 block move right
        }
    }
}

void MaskMover::next_cases(RawCode raw_code, uint64_t mask) { // search next step cases
    const uint64_t code = raw_code.unwrap();
    cache_[0].filter = 0; // without filter
    cache_[0].code = code; // bfs root code
    auto range = code | mask;

    for (int addr = 0; range; addr += 3, range >>= 3) { // traverse every 3-bits
        switch (range & 0b111) { // match low 3-bits
            case BLOCK_1x1:
                move_1x1(code, addr); // try to move 1x1 block
                break;
            case BLOCK_1x2:
                move_1x2(code, addr); // try to move 1x2 block
                break;
            case BLOCK_2x1:
                move_2x1(code, addr); // try to move 2x1 block
                break;
            case BLOCK_2x2:
                move_2x2(code, addr); // try to move 2x2 block
                break;
            default:
                continue; // B_space or B_fill
        }
        if (cache_size_ != 1) { // found one or more next cases
            for (int i = 1; i < cache_size_; ++i) {
                release_(RawCode::unsafe_create(cache_[i].code), cache_[i].mask); // release next cases
            }
            cache_size_ = 1; // reset cache size
        }
    }
}
