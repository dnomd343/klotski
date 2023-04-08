#pragma once

/// AllCases can generate all valid CommonCodes, a total of 29334498 kinds.
/// For the generated BasicRanges, use different `2x2` block positions to
/// check respectively. On the `5x4` chessboard, it has 12 possible positions,
/// and the numbers are distributed in 0 ~ 15, witch called `head`. For the
/// convenience of calculation, here use an array of length 16 for storage,
/// of course, the position of 3/7/11/15 will be empty.

///   00 01 02 03
///   04 05 06 07    00 01 02
///   08 09 10 11    04 05 06  <- head of 2x2 block
///   12 13 14 15    08 09 10
///   16 17 18 19    12 13 14

/// After checking, each head has a different valid `range`, and they are
/// stored in different arrays to save memory (otherwise the 64-bit length
/// must be consumed).

#include <mutex>
#include <vector>
#include <cstdint>
#include <numeric>
#include "common_code.h"
#include "basic_ranges.h"

namespace klotski {

class CommonCode;

/// all cases count
const uint32_t ALL_CASES_SIZE[16] = {
    2942906, 2260392, 2942906, 0,
    2322050, 1876945, 2322050, 0,
    2322050, 1876945, 2322050, 0,
    2942906, 2260392, 2942906, 0,
};

const uint32_t ALL_CASES_SIZE_SUM = 29334498;

class AllCases : public BasicRanges {
public:
    /// Trigger the build process.
    static void build();

    /// Get current status of AllCases.
    static Status status() noexcept;

    /// Blocking access to constructed data.
    static const std::vector<uint32_t> (&fetch())[16];

    /// Export all possible common codes.
    static std::vector<CommonCode> release();

private:
    static bool available_;
    static std::mutex building_;
    static std::vector<uint32_t> data_[16];

    static void build_data();
};

} // namespace klotski
