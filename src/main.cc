#include "case.h"
#include "klotski.h"
#include <cstdio>
#include <cstdint>
#include <list>
#include <unordered_map>
#include <queue>

#define UP   (-12)
#define LEFT  (-3)
#define DOWN   12
#define DOWN_2 24
#define RIGHT   3
#define RIGHT_2 6

struct cache {
    uint64_t code;
    uint64_t filter;
};
struct cache move_cache[16];
int move_cache_num;

struct klotski_info {
    int step;
    uint64_t code;
    uint64_t filter;
    std::list<klotski_info*> src;
};

std::queue<klotski_info*> cal_cache;
std::unordered_map<uint64_t, klotski_info*> klotski_case;

void graph_output(uint64_t code) {
    for (int i = 0; i < 20; ++i) {
        switch (code & 0x7) {
            case B_1x1:
                printf("# ");
                break;
            case B_1x2:
                printf("& ");
                break;
            case B_2x1:
                printf("$ ");
                break;
            case B_2x2:
                printf("@ ");
                break;
            case B_fill:
                printf("* ");
                break;
            case B_space:
                printf(". ");
                break;
            default:
                printf("? ");
        }
        if ((i & 0x3) == 0x3) {
            printf("\n");
        }
        code >>= 3;
    }
}


inline bool move_block_release(uint64_t code, uint64_t filter) {
    struct cache *p = move_cache;
    for (; p < move_cache + move_cache_num; ++p) {
        if (p->code == code) {
            return false;
        }
    }
    p->code = code;
    p->filter = filter;
    ++move_cache_num;
    return true;
}

void move_block_1x1(uint64_t code, int addr, int filter) {
    int target_addr;
    uint64_t ret_code;
    if (filter != UP && addr >= 4 * 3 && !(code >> (target_addr = addr + UP) & F_1x1)) {
        ret_code = code & ~(F_1x1 << addr) | C_1x1 << target_addr;
        if (move_block_release(ret_code, uint64_t(0x7) << target_addr)) {
            move_block_1x1(ret_code, target_addr, DOWN);
        }
    }
    if (filter != DOWN && addr <= 15 * 3 && !(code >> (target_addr = addr + DOWN) & F_1x1)) {
        ret_code = code & ~(F_1x1 << addr) | C_1x1 << target_addr;
        if (move_block_release(ret_code, uint64_t(0x7) << target_addr)) {
            move_block_1x1(ret_code, target_addr, UP);
        }
    }
    if (filter != LEFT && (addr & 0x3) != 0 && !(code >> (target_addr = addr + LEFT) & F_1x1)) {
        ret_code = code & ~(F_1x1 << addr) | C_1x1 << target_addr;
        if (move_block_release(ret_code, uint64_t(0x7) << target_addr)) {
            move_block_1x1(ret_code, target_addr, RIGHT);
        }
    }
    if (filter != RIGHT && (addr & 0x3) != 1 && !(code >> (target_addr = addr + RIGHT) & F_1x1)) {
        ret_code = code & ~(F_1x1 << addr) | C_1x1 << target_addr;
        if (move_block_release(ret_code, uint64_t(0x7) << target_addr)) {
            move_block_1x1(ret_code, target_addr, LEFT);
        }
    }
}

void move_block_1x2(uint64_t code, int addr, int filter) {
    int target_addr;
    uint64_t ret_code;
    if (filter != UP && addr >= 4 * 3 && !(code >> (target_addr = addr + UP) & F_1x2)) {
        ret_code = code & ~(F_1x2 << addr) | C_1x2 << target_addr;
        if (move_block_release(ret_code, uint64_t(0x7) << target_addr)) {
            move_block_1x2(ret_code, target_addr, DOWN);
        }
    }
    if (filter != DOWN && addr <= 14 * 3 && !(code >> (target_addr = addr + DOWN) & F_1x2)) {
        ret_code = code & ~(F_1x2 << addr) | C_1x2 << target_addr;
        if (move_block_release(ret_code, uint64_t(0x7) << target_addr)) {
            move_block_1x2(ret_code, target_addr, UP);
        }
    }
    if (filter != LEFT && (addr & 0x3) != 0 && !(code >> (target_addr = addr + LEFT) & F_1x1)) {
        ret_code = code & ~(F_1x2 << addr) | C_1x2 << target_addr;
        if (move_block_release(ret_code, uint64_t(0x7) << target_addr)) {
            move_block_1x2(ret_code, target_addr, RIGHT);
        }
    }
    if (filter != RIGHT && (addr & 0x3) != 2 && !(code >> (addr + RIGHT_2) & F_1x1)) {
        ret_code = code & ~(F_1x2 << addr) | C_1x2 << (target_addr = addr + RIGHT);
        if (move_block_release(ret_code, uint64_t(0x7) << target_addr)) {
            move_block_1x2(ret_code, target_addr, LEFT);
        }
    }
}

void move_block_2x1(uint64_t code, int addr, int filter) {
    int target_addr;
    uint64_t ret_code;
    if (filter != UP && addr >= 4 * 3 && !(code >> (target_addr = addr + UP) & F_1x1)) {
        ret_code = code & ~(F_2x1 << addr) | C_2x1 << target_addr;
        if (move_block_release(ret_code, uint64_t(0x7) << target_addr)) {
            move_block_2x1(ret_code, target_addr, DOWN);
        }
    }
    if (filter != DOWN && addr <= 11 * 3 && !(code >> (addr + DOWN_2) & F_1x1)) {
        ret_code = code & ~(F_2x1 << addr) | C_2x1 << (target_addr = addr + DOWN);
        if (move_block_release(ret_code, uint64_t(0x7) << target_addr)) {
            move_block_2x1(ret_code, target_addr, UP);
        }
    }
    if (filter != LEFT && (addr & 0x3) != 0 && !(code >> (target_addr = addr + LEFT) & F_2x1)) {
        ret_code = code & ~(F_2x1 << addr) | C_2x1 << target_addr;
        if (move_block_release(ret_code, uint64_t(0x7) << target_addr)) {
            move_block_2x1(ret_code, target_addr, RIGHT);
        }
    }
    if (filter != RIGHT && (addr & 0x3) != 1 && !(code >> (target_addr = addr + RIGHT) & F_2x1)) {
        ret_code = code & ~(F_2x1 << addr) | C_2x1 << target_addr;
        if (move_block_release(ret_code, uint64_t(0x7) << target_addr)) {
            move_block_2x1(ret_code, target_addr, LEFT);
        }
    }
}

void move_block_2x2(uint64_t code, int addr, int filter) {
    int target_addr;
    uint64_t ret_code;
    if (filter != UP && addr >= 4 * 3 && !(code >> (target_addr = addr + UP) & F_1x2)) {
        ret_code = code & ~(F_2x2 << addr) | C_2x2 << target_addr;
        if (move_block_release(ret_code, uint64_t(0x7) << target_addr)) {
            move_block_2x2(ret_code, target_addr, DOWN);
        }
    }
    if (filter != DOWN && addr <= 10 * 3 && !(code >> (addr + DOWN_2) & F_1x2)) {
        ret_code = code & ~(F_2x2 << addr) | C_2x2 << (target_addr = addr + DOWN);
        if (move_block_release(ret_code, uint64_t(0x7) << target_addr)) {
            move_block_2x2(ret_code, target_addr, UP);
        }
    }
    if (filter != LEFT && (addr & 0x3) != 0 && !(code >> (target_addr = addr + LEFT) & F_2x1)) {
        ret_code = code & ~(F_2x2 << addr) | C_2x2 << target_addr;
        if (move_block_release(ret_code, uint64_t(0x7) << target_addr)) {
            move_block_2x2(ret_code, target_addr, RIGHT);
        }
    }
    if (filter != RIGHT && (addr & 0x3) != 2 && !(code >> (addr + RIGHT_2) & F_2x1)) {
        ret_code = code & ~(F_2x2 << addr) | C_2x2 << (target_addr = addr + RIGHT);
        if (move_block_release(ret_code, uint64_t(0x7) << target_addr)) {
            move_block_2x2(ret_code, target_addr, LEFT);
        }
    }
}

void add_new_case(klotski_info *src, uint64_t code, uint64_t filter) {
    auto existing_case = klotski_case.find(code);
    if (existing_case != klotski_case.end()) {
        existing_case->second->filter |= filter;
        if (existing_case->second->step != src->step) {
            existing_case->second->src.push_back(src);
        }
        return;
    }

    auto info = new struct klotski_info;
    info->code = code;
    info->filter = filter;
    info->step = src->step + 1;
    info->src.push_back(src);
    cal_cache.emplace(info);
    klotski_case.emplace(code, info);
}

void next_step(klotski_info *klotski) {
    move_cache->code = klotski->code;
    uint64_t range = move_cache->code & ~klotski->filter;
    for (int addr = 0; range; range >>= 3, addr += 3) {
        move_cache_num = 1;
        switch (range & 0x7) {
            case B_2x2:
                move_block_2x2(move_cache->code, addr, 0);
                break;
            case B_2x1:
                move_block_2x1(move_cache->code, addr, 0);
                break;
            case B_1x2:
                move_block_1x2(move_cache->code, addr, 0);
                break;
            case B_1x1:
                move_block_1x1(move_cache->code, addr, 0);
                break;
            default:
                continue;
        }
        for (struct cache *p = move_cache + 1; p < move_cache + move_cache_num; ++p) {
            add_new_case(klotski, p->code, p->filter);
        }
    }
}

void cal_klotski(uint64_t code) {

    auto setup = new klotski_info;
    setup->step = 0;
    setup->code = code;
    setup->filter = 0x0;

    klotski_case.clear();
    klotski_case.emplace(code, setup);

    cal_cache.push(setup);
    while (!cal_cache.empty()) {
        next_step(cal_cache.front());
        cal_cache.pop();
    }

//    printf("count -> %zu\n", klotski_case.size());

}

int main() {
    printf("Klotski engine\n");

//    printf("%lx\n", compact_code(0x0E58FC85FFEBC4DB));
//    printf("%lx\n", compact_code(0x0603EDF5CAFFF5E2));

//    graph_output(extract_code(0x4FEA13400));
//    printf("\n");
//    graph_output(extract_code(0x1A9BF0C00));
//    printf("\n");

//    std::vector<uint64_t> all_case;
//    find_all_case(&all_case);
//    printf("count -> %lu\n", all_case.size());
//
//    for (auto code : all_case) {
//        if (code != compact_code(extract_code(code))) {
//            printf("error -> %lx\n", code);
//        }
//    }

    // 0x4FEA13400
    //  # # # @  |  011 011 011 010 => 0100 1101 1011 -> 4DB
    //  * * & @  |  100 111 010 111 => 1110 1011 1100 -> EBC
    //  * * & $  |  111 111 111 010 => 0101 1111 1111 -> 5FF
    //  . + + $  |  000 001 111 111 => 1111 1100 1000 -> FC8
    //  . # ~ ~  |  000 011 001 111 => 1110 0101 1000 -> E58
    // 0x0E58FC85FFEBC4DB

    // 0x1A9BF0C00
    //  @ * * @  |  010 100 111 010 => 0101 1110 0010 -> 5E2
    //  @ * * @  |  111 111 111 111 => 1111 1111 1111 -> FFF
    //  $ ~ ~ $  |  010 001 111 010 => 0101 1100 1010 -> 5CA
    //  $ # # $  |  111 011 011 111 => 1110 1101 1111 -> EDF
    //  # . . #  |  011 000 000 011 => 0110 0000 0011 -> 603
    // 0x0603EDF5CAFFF5E2


//    uint64_t code = 0x0603EDF5CAFFF5E2;
    uint64_t code = 0x0E58FC85FFEBC4DB;

    for (int i = 0; i < 100; ++i) {
        cal_klotski(code);
    }

    return 0;
}
