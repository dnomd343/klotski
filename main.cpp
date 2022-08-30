#include <cstdio>
#include <cstdint>

#define B_empty 0x6 // undefined case
#define B_space 0x0
#define B_fill  0x7
#define B_1x2   0x1
#define B_2x1   0x2
#define B_1x1   0x3
#define B_2x2   0x4

#define C_1x1 int64_t(0x3)
#define C_1x2 int64_t(0x39)
#define C_2x1 int64_t(0x7002)
#define C_2x2 int64_t(0x3F03C)

#define F_1x1 int64_t(0x7)
#define F_1x2 int64_t(0x3F)
#define F_2x1 int64_t(0x7007)
#define F_2x2 int64_t(0x3F03F)

/*

  uint64_t -> 0000 + [xxx] * 20

  2x2  2x1  1x2  1x1
  # #   #   # #  #
  # #   #

  00 01 02 03
  04 05 06 07
  08 09 10 11
  12 13 14 15
  16 17 18 19

  x1 (%4) => 0 1 2 3
  x3 (%4) => 0 3 2 1

*/

/*
======================================================

 1x1 -> 011 000 000 000 -> 0000 0000 0011 -> 0x3

 1x2 -> 001 111 000 000 -> 0000 0011 1001 -> 0x39

 2x1 -> 010 000 000 000 -> 0000 0000 0010 -> 0x7002
        111 000 000 000 -> 0000 0000 0111

 2x2 -> 100 111 000 000 -> 0000 0011 1100 -> 0x3F03C
        111 111 000 000 -> 0000 0011 1111

======================================================

 1x1 -> 111 000 000 000 -> 0000 0000 0111 -> 0x7

 1x2 -> 111 111 000 000 -> 0000 0011 1111 -> 0x3F

 2x1 -> 111 000 000 000 -> 0000 0000 0111 -> 0x7007
        111 000 000 000 -> 0000 0000 0111

 2x2 -> 111 111 000 000 -> 0000 0011 1111 -> 0x3F03F
        111 111 000 000 -> 0000 0011 1111

======================================================
*/

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

uint64_t compact_code(uint64_t code) {
    int block_num = 0;
    uint64_t ret = 0;
    uint32_t range = 0;
    for (int addr = 0; code; ++addr, code >>= 3) {
        switch (code & 0x7) {
            case B_space:
                range <<= 2;
                break;
            case B_1x2:
                (range <<= 2) |= 0x1;
                break;
            case B_2x1:
                (range <<= 2) |= 0x2;
                break;
            case B_1x1:
                (range <<= 2) |= 0x3;
                break;
            case B_2x2:
                ret |= uint64_t(addr) << 32;
            default:
                continue;
        }
        ++block_num;
    }
    return ret | range << (16 - block_num) * 2;
}

int main() {
    printf("Klotski engine\n");

    printf("%lx\n", compact_code(0x0E58FC85FFEBC4DB));
    printf("%lx\n", compact_code(0x0603EDF5CAFFF5E2));

    return 0;

    // 0x4FEA13400
    //  # # # @  |  011 011 011 010 => 0100 1101 1011 -> 4DB
    //  * * & @  |  100 111 010 111 => 1110 1011 1100 -> EBC
    //  * * & $  |  111 111 111 010 => 0101 1111 1111 -> 5FF
    //  . + + $  |  000 001 111 111 => 1111 1100 1000 -> FC8
    //  . # ~ ~  |  000 011 001 111 => 1110 0101 1000 -> E58
    // 0x0E58FC85FFEBC4DB

    graph_output(0x0E58FC85FFEBC4DB);
    printf("\n");

    // 0x1A9BF0C00
    //  @ * * @  |  010 100 111 010 => 0101 1110 0010 -> 5E2
    //  @ * * @  |  111 111 111 111 => 1111 1111 1111 -> FFF
    //  $ ~ ~ $  |  010 001 111 010 => 0101 1100 1010 -> 5CA
    //  $ # # $  |  111 011 011 111 => 1110 1101 1111 -> EDF
    //  # . . #  |  011 000 000 011 => 0110 0000 0011 -> 603
    // 0x0603EDF5CAFFF5E2

    graph_output(0x0603EDF5CAFFF5E2);
    printf("\n");

    return 0;
}
