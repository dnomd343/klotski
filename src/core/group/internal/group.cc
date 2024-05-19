#include "group/group.h"

static KLSK_INLINE uint32_t type_id(const int n, const int n_2x1, const int n_1x1) {
    constexpr int offset[8] = {0, 15, 41, 74, 110, 145, 175, 196};
    return offset[n] + (15 - n * 2) * n_2x1 + n_1x1;
}

uint32_t klotski::cases::common_code_to_type_id(const uint64_t common_code) {
    const auto range = static_cast<uint32_t>(common_code);
    const auto n_1x1 = std::popcount((range >> 1) & range & 0x55555555);
    const auto n_2x1 = std::popcount((range >> 1) & ~range & 0x55555555);
    return type_id(std::popcount(range) - n_1x1 * 2, n_2x1, n_1x1);
}

uint32_t klotski::cases::raw_code_to_type_id(const uint64_t raw_code) {
    const auto n = std::popcount(((raw_code >> 1) ^ raw_code) & 0x0249249249249249);
    const auto n_2x1 = std::popcount((raw_code >> 1) & ~raw_code & 0x0249249249249249);
    const auto n_1x1 = std::popcount((raw_code >> 1) & raw_code & 0x0249249249249249) - n - 3;
    return type_id(n, n_2x1, n_1x1);
}
