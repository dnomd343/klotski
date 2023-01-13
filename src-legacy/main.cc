//#include "case.h"
//#include "klotski.h"
#include <cstdio>
#include <cstdint>
#include <list>
#include <unordered_map>
#include <queue>

//#define UP   (-12)
//#define LEFT  (-3)
//#define DOWN   12
//#define DOWN_2 24
//#define RIGHT   3
//#define RIGHT_2 6

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

//    for (int i = 0; i < 100; ++i) {
        cal_klotski(code);
//    }

    return 0;
}
