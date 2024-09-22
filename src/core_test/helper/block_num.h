#pragma once

#include <cstdint>

// ----------------------------------------------------------------------------------------- //

namespace helper {

struct block_num_t {
    int n_1x1; // 4-bit
    int n_1x2; // 3-bit
    int n_2x1; // 3-bit
};

constexpr bool operator==(const block_num_t &lhs, const block_num_t &rhs) {
    return lhs.n_1x1 == rhs.n_1x1
        && lhs.n_1x2 == rhs.n_1x2
        && lhs.n_2x1 == rhs.n_2x1;
}

/// Calculate the block number from Range.
block_num_t cal_block_num(uint32_t range);

/// Calculate type id value from the block number.
uint32_t to_type_id(block_num_t block_num);

/// Calculate the block number value from type id.
block_num_t to_block_num(uint32_t type_id);

/// Get all block number combinations without dependencies.
const std::vector<block_num_t>& block_nums();

} // namespace helper

// ----------------------------------------------------------------------------------------- //

namespace std {

template <>
struct std::hash<helper::block_num_t> {
    size_t operator()(const helper::block_num_t val) const noexcept {
        return (val.n_1x1 << 6) ^ (val.n_1x2 << 3) ^ val.n_2x1;
    }
};

} // namespace std

// ----------------------------------------------------------------------------------------- //
