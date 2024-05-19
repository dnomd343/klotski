#include "group/group.h"

#include <utils/common.h>

/// 1. n_1x1 + (n_1x2 + n_2x1) * 2 <= 14
/// 2. (n_1x1 != 0) && (n_2x1 != 7)
struct block_num_t {
    uint8_t n_1x1 = 0; /// [0, 14]
    uint8_t n_1x2 = 0; /// [0, 7]
    uint8_t n_2x1 = 0; /// [0, 7]
};
/// n_space = 16 - n_1x1 - (n_1x2 + n_2x1) * 2

// TODO: fast convert from RawCode / CommonCode -> block_num_t
// static block_num_t block_num(klotski::codec::RawCode raw_code);
// static block_num_t block_num(klotski::codec::CommonCode common_code);

// TODO: convert from block_num -> type_id
// static int type_id(block_num_t block_num);

using klotski::range_reverse;

const uint16_t TYPE_ID_INDEX[203] = {
       0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,  256,
     257,  258,  259,  260,  261,  262,  263,  264,  265,  266,  267,  268,  272,  273,  274,  275,
     276,  277,  278,  279,  280,  281,  282,  283,  284,  512,  513,  514,  515,  516,  517,  518,
     519,  520,  521,  522,  528,  529,  530,  531,  532,  533,  534,  535,  536,  537,  538,  544,
     545,  546,  547,  548,  549,  550,  551,  552,  553,  554,  768,  769,  770,  771,  772,  773,
     774,  775,  776,  784,  785,  786,  787,  788,  789,  790,  791,  792,  800,  801,  802,  803,
     804,  805,  806,  807,  808,  816,  817,  818,  819,  820,  821,  822,  823,  824, 1024, 1025,
    1026, 1027, 1028, 1029, 1030, 1040, 1041, 1042, 1043, 1044, 1045, 1046, 1056, 1057, 1058, 1059,
    1060, 1061, 1062, 1072, 1073, 1074, 1075, 1076, 1077, 1078, 1088, 1089, 1090, 1091, 1092, 1093,
    1094, 1280, 1281, 1282, 1283, 1284, 1296, 1297, 1298, 1299, 1300, 1312, 1313, 1314, 1315, 1316,
    1328, 1329, 1330, 1331, 1332, 1344, 1345, 1346, 1347, 1348, 1360, 1361, 1362, 1363, 1364, 1536,
    1537, 1538, 1552, 1553, 1554, 1568, 1569, 1570, 1584, 1585, 1586, 1600, 1601, 1602, 1616, 1617,
    1618, 1632, 1633, 1634, 1792, 1808, 1824, 1840, 1856, 1872, 1888,
};

using klotski::cases::TYPE_ID_LIMIT;

uint32_t cal_type_id(block_num_t &&block_num) noexcept { // block_num_t -> type_id
    /// flag -> ... 0000  0xxx  0xxx  xxxx
    ///                  n_x2x n_2x1 n_1x1
    auto n_x2x = block_num.n_1x2 + block_num.n_2x1;
    auto flag = (n_x2x << 8) | (block_num.n_2x1 << 4) | block_num.n_1x1;
    return std::lower_bound(TYPE_ID_INDEX, TYPE_ID_INDEX + TYPE_ID_LIMIT, flag) - TYPE_ID_INDEX;
}

block_num_t cal_common_block_num(const uint64_t common_code) noexcept {
    block_num_t result;
    auto range = range_reverse(static_cast<uint32_t>(common_code));
    for (; range; range >>= 2) {
        switch (range & 0b11) {
        case 0b01: /// 1x2 block
            ++result.n_1x2;
            continue;
        case 0b10: /// 2x1 block
            ++result.n_2x1;
            continue;
        case 0b11: /// 1x1 block
            ++result.n_1x1;
            continue;
        }
    }
    return result;
}

block_num_t cal_raw_block_num(const uint64_t raw_code) noexcept {
    block_num_t result;
    auto tmp = raw_code;
    for (int addr = 0; addr < 20; ++addr, tmp >>= 3) {
        switch (tmp & 0b111) {
        case BLOCK_1x1:
            ++result.n_1x1;
            continue;
        case BLOCK_1x2:
            ++result.n_1x2;
            continue;
        case BLOCK_2x1:
            ++result.n_2x1;
            continue;
        }
    }
    return result;
}

uint32_t my_type_id(uint32_t n_1x2, uint32_t n_2x1, uint32_t n_1x1) noexcept { // block_num_t -> type_id
    /// flag -> ... 0000  0xxx  0xxx  xxxx
    ///                  n_x2x n_2x1 n_1x1
    // auto n_x2x = n_1x2 + n_2x1;
    // auto flag = (n_x2x << 8) | (n_2x1 << 4) | n_1x1;
    // return std::lower_bound(TYPE_ID_INDEX, TYPE_ID_INDEX + TYPE_ID_LIMIT, flag) - TYPE_ID_INDEX;

    // for (int n = 0; n <= 7; ++n) { // n -> n_1x2 + n_2x1
    //     for (int n_21 = 0; n_21 <= n; ++n) {
    //         // n_11 <= 14 - n * 2
    //         for (int n_11 = 0; n_11 <= (14 - n*2); ++n_11) {
    //             // get one case
    //         }
    //     }
    // }

    // n = 0 | n_21 ~ 1 | n_11 ~ 15 | => 15
    // n = 1 | n_21 ~ 2 | n_11 ~ 13 | => 26
    // n = 2 | n_21 ~ 3 | n_11 ~ 11 | => 33
    // n = 3 | n_21 ~ 4 | n_11 ~ 9  | => 36
    // n = 4 | n_21 ~ 5 | n_11 ~ 7  | => 35
    // n = 5 | n_21 ~ 6 | n_11 ~ 5  | => 30
    // n = 6 | n_21 ~ 7 | n_11 ~ 3  | => 21
    // n = 7 | n_21 ~ 8 | n_11 ~ 1  | => 8

    constexpr uint32_t offset_tab[8] = {0, 15, 41, 74, 110, 145, 175, 196};

    uint32_t n = n_1x2 + n_2x1;
    uint32_t offset = offset_tab[n];

    auto span = 15 - n*2;

    uint32_t offset_ = span * n_2x1;

    return offset + offset_ + n_1x1;

}

uint32_t common_code_pro(uint64_t common_code) {
    uint32_t range = (uint32_t)common_code;
    uint32_t k_01 = (~range >> 1) & range & 0x55555555;
    uint32_t k_10 = (range >> 1) & ~range & 0x55555555;
    uint32_t k_11 = (range >> 1) & range & 0x55555555;

    auto n_01 = std::popcount(k_01);
    auto n_10 = std::popcount(k_10);
    auto n_11 = std::popcount(k_11);

    // block_num_t tmp {
    //     .n_1x1 = (uint8_t)std::popcount(k_11),
    //     .n_1x2 = (uint8_t)std::popcount(k_01),
    //     .n_2x1 = (uint8_t)std::popcount(k_10),
    // };

    return my_type_id(n_01, n_10, n_11);

    // return my_type_id(std::popcount(k_01), std::popcount(k_10), std::popcount(k_11));
}

uint32_t klotski::cases::common_code_to_type_id(uint64_t common_code) {
    // return cal_type_id(cal_common_block_num(common_code));
    return common_code_pro(common_code);
}

uint32_t klotski::cases::raw_code_to_type_id(uint64_t raw_code) {
    return cal_type_id(cal_raw_block_num(raw_code));
}
