#include <utils/common.h>
#include <gtest/gtest.h>

#include <thread>

#include <md5.h>

#include "group/group.h"
#include "all_cases/all_cases.h"

using klotski::range_reverse;

using klotski::cases::AllCases;
using klotski::codec::CommonCode;

using klotski::cases::ALL_CASES_NUM;

struct block_num_t {
    uint8_t n_1x1 = 0; /// [0, 14]
    uint8_t n_1x2 = 0; /// [0, 7]
    uint8_t n_2x1 = 0; /// [0, 7]
};

bool operator==(block_num_t b1, block_num_t b2) {
    return (b1.n_1x1 == b2.n_1x1) && (b1.n_1x2 == b2.n_1x2) && (b1.n_2x1 == b2.n_2x1);
}

const char BLOCK_NUM_MD5[] = "46a7b3af6d039cbe2f7eaebdd196c6a2";

block_num_t common_block_num(const uint64_t common_code) noexcept {
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

block_num_t raw_block_num(const uint64_t raw_code) noexcept {
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

uint32_t get_type_id(block_num_t block_num) noexcept { // block_num_t -> type_id
    /// flag -> ... 0000  0xxx  0xxx  xxxx
    ///                  n_x2x n_2x1 n_1x1
    auto n_x2x = block_num.n_1x2 + block_num.n_2x1;
    auto flag = (n_x2x << 8) | (block_num.n_2x1 << 4) | block_num.n_1x1;
    return std::lower_bound(TYPE_ID_INDEX, TYPE_ID_INDEX + klotski::cases::TYPE_ID_LIMIT, flag) - TYPE_ID_INDEX;
}

TEST(Group, block_num) {
    std::string result[16];
    auto test = [&result](uint64_t head) {
        char buffer[13];
        result[head].reserve(ALL_CASES_NUM[head]); // vector pre-allocated

        for (const auto range: AllCases::instance().fetch()[head]) {
            auto common_code = CommonCode::unsafe_create(head << 32 | range);

            auto tmp = common_block_num(common_code.unwrap());
            EXPECT_EQ(tmp, raw_block_num(common_code.to_raw_code().unwrap()));

            EXPECT_LE(tmp.n_1x2 * 2 + tmp.n_2x1 * 2 + tmp.n_1x1, 14);
            sprintf(buffer, "%d,%d,%d\n", tmp.n_1x2 + tmp.n_2x1, tmp.n_1x1, tmp.n_2x1);
            result[head] += buffer;
        }
    };

    std::thread threads[16];
    for (uint64_t head = 0; head < 16; ++head) {
        threads[head] = std::thread(test, head); // multi-threads verify
    }
    for (auto &t : threads) { t.join(); } // build string data

    std::string block_num_str;
    for (auto &&tmp : result) {
        block_num_str += tmp; // combine result
    }
    auto block_num_md5 = md5::MD5::Hash(block_num_str.c_str(), block_num_str.size());
    EXPECT_STREQ(block_num_md5.c_str(), BLOCK_NUM_MD5); // verify md5
}

TEST(Group, common_code) {

    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto range : AllCases::instance().fetch()[head]) {

            auto common_code = (head << 32 | range);

            // EXPECT_EQ(klotski::cases::common_code_to_type_id(common_code), get_type_id(common_block_num(common_code)));

        }
    }

}

TEST(Group, raw_code) {
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto range : AllCases::instance().fetch()[head]) {

            auto raw_code = CommonCode::unsafe_create(head << 32 | range).to_raw_code().unwrap();

            // EXPECT_EQ(klotski::cases::raw_code_to_type_id(raw_code), get_type_id(raw_block_num(raw_code)));

        }
    }

}
