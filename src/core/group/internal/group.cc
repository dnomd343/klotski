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

using klotski::range_reverse;

uint32_t my_type_id(uint32_t n_1x2, uint32_t n_2x1, uint32_t n_1x1) noexcept { // block_num_t -> type_id

    // n = 0 | n_21 ~ 1 | n_11 ~ 15 | => 15
    // n = 1 | n_21 ~ 2 | n_11 ~ 13 | => 26
    // n = 2 | n_21 ~ 3 | n_11 ~ 11 | => 33
    // n = 3 | n_21 ~ 4 | n_11 ~ 9  | => 36
    // n = 4 | n_21 ~ 5 | n_11 ~ 7  | => 35
    // n = 5 | n_21 ~ 6 | n_11 ~ 5  | => 30
    // n = 6 | n_21 ~ 7 | n_11 ~ 3  | => 21
    // n = 7 | n_21 ~ 8 | n_11 ~ 1  | => 8

    // input -> n / n_2x1 / n_1x1

    constexpr uint32_t offset[8] = {0, 15, 41, 74, 110, 145, 175, 196};

    uint32_t n = n_1x2 + n_2x1;

    return offset[n] + (15 - n*2) * n_2x1 + n_1x1;

}

uint32_t type_id_pro(int n, int n_2x1, int n_1x1) {

    constexpr uint32_t offset[8] = {0, 15, 41, 74, 110, 145, 175, 196};

    return offset[n] + (15 - n*2) * n_2x1 + n_1x1;

}

uint32_t common_code_pro(uint64_t common_code) {
    const auto range = static_cast<uint32_t>(common_code);
    const auto n_1x1 = std::popcount((range >> 1) & range & 0x55555555);
    const auto n_2x1 = std::popcount((range >> 1) & ~range & 0x55555555);
    return type_id_pro(std::popcount(range) - n_1x1 * 2, n_2x1, n_1x1);
}

uint32_t raw_code_pro(uint64_t raw_code) {
    // 1x1 -> 011
    // 1x2 -> 001
    // 2x1 -> 010

    ///    0    2    4    9    2    4    9    2    4    9    2    4    9    2    4    9
    /// 0000 0010 0100 1001 0010 0100 1001 0010 0100 1001 0010 0100 1001 0010 0100 1001
    uint64_t k_1x1 = (~raw_code >> 2) & (raw_code >> 1) & raw_code & (uint64_t)0x0249249249249249;
    // uint64_t k_1x2 = (~raw_code >> 2) & (~raw_code >> 1) & raw_code & (uint64_t)0x0249249249249249;
    // uint64_t k_2x1 = (~raw_code >> 2) & (raw_code >> 1) & ~raw_code & (uint64_t)0x0249249249249249;

    // 0010 0100 1001 -> 249
    // uint64_t k_1x2 = (~raw_code >> 1) & raw_code & (uint64_t)0x0249249249249249;
    uint64_t k_2x1 = (raw_code >> 1) & ~raw_code & (uint64_t)0x0249249249249249;

    auto n_1x1 = std::popcount(k_1x1);
    // auto n_1x2 = std::popcount(k_1x2);
    auto n_2x1 = std::popcount(k_2x1);

    uint64_t k_ = ((raw_code >> 1) ^ raw_code) & (uint64_t)0x0249249249249249;
    auto n_ = std::popcount(k_);

    // popcount = 10 + BLOCK_1x1 * 2 + (BLOCK_1x2 + BLOCK_2x1) * 4
    // auto n_1x1 = (std::popcount(raw_code) - 10 - (n_1x2 + n_2x1) * 4) / 2;

    uint64_t k_1x1_ = (raw_code >> 1) & raw_code & (uint64_t)0x0249249249249249;
    auto n_1x1_ = std::popcount(k_1x1_) - n_ - 3;

    // 0100 1001 0010 -> 492
    uint64_t k_1x1__ = (~raw_code >> 1) & raw_code & (uint64_t)0x0492492492492492;
    auto n_1x1__ = std::popcount(k_1x1__) - n_2x1;

    // return my_type_id(n_1x2, n_2x1, n_1x1);
    // return type_id_pro(n_2x1 + n_1x2, n_2x1, n_1x1);
    return type_id_pro(n_, n_2x1, n_1x1_);
}

uint32_t klotski::cases::common_code_to_type_id(uint64_t common_code) {
    // return cal_type_id(cal_common_block_num(common_code));
    return common_code_pro(common_code);
}

uint32_t klotski::cases::raw_code_to_type_id(uint64_t raw_code) {
    // return cal_type_id(cal_raw_block_num(raw_code));
    return raw_code_pro(raw_code);
}
