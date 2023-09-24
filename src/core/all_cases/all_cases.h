#pragma once

/// Based on the requirements of valid klotski, the 2x2 block that must exist
/// and only one, witch will occupy 4 empty slots, and the remaining 16 slots
/// will be allocated to 1x2, 2x1, 1x1 and space. Then, according to the rules
/// of CommonCode, they are coded as `00`, `01`, `10`, `11` respectively, and
/// the remaining positions are filled with 0 and stored as 32-bit variables.

/// As we all know, a space or 1x1 block will occupy 1 slot, 1x2 and 2x1 block
/// will occupy 2 slots, and together they fill 16 positions, so all possible
/// combinations can be calculated, this number is 204, each combination can
/// produce different permutations. After mathematical calculations, there are
/// a total of 7311921 possible permutations. The goal of BasicRanges is to
/// find these permutations, sort them and store in a uint32_t array.

/// AllCases is used to generate all valid CommonCodes, its works based on all
/// permutations generated by BasicRanges, which will use different 2x2 block
/// positions to check respectively. On the 5x4 chessboard, it has 12 possible
/// positions, and the numbers are distributed in 0 ~ 15, witch called `head`.
/// For the convenience of calculation, here use an array of length 16 for
/// storage, of course, the position of 3/7/11/15 will be empty.

///   00 01 02 03
///   04 05 06 07    00 01 02
///   08 09 10 11    04 05 06  <- head of 2x2 block
///   12 13 14 15    08 09 10
///   16 17 18 19    12 13 14

/// After all the work is done, we will have 29334498 cases, distributed in 16
/// arrays. Each of them is a uint32_t array storing the ranges, this is to
/// save memory, otherwise the 64-bit length must be consumed.

/// By the way, due to the performance considerations of the checking process,
/// the resulting data of BasicRanges will be flipped every 2-bit, which will
/// not consume too much time, but it can almost double the speed of the case
/// checking subsequent.

#include <array>
#include <mutex>
#include <vector>
#include <cstdint>
#include <numeric>
#include "utility.h"

namespace klotski {
namespace cases {

typedef uint32_t Range;
typedef std::vector<Range> Ranges;
typedef std::array<Ranges, 16> RangesUnion;
typedef std::function<void(std::function<void()>&&)> Executor;

constexpr auto BASIC_RANGES_NUM = 7311921;

constexpr std::array<int, 16> ALL_CASES_NUM {
    2942906, 2260392, 2942906, 0,
    2322050, 1876945, 2322050, 0,
    2322050, 1876945, 2322050, 0,
    2942906, 2260392, 2942906, 0,
};

constexpr auto ALL_CASES_NUM_ = std::accumulate(
    ALL_CASES_NUM.begin(), ALL_CASES_NUM.end(), 0
);

class BasicRanges {
public:
    void Build() noexcept;
    const Ranges& Fetch() noexcept;
    bool IsAvailable() const noexcept;

    DISALLOW_COPY_AND_ASSIGN(BasicRanges);
    static BasicRanges& Instance() noexcept;

private:
    std::mutex building_;
    bool available_ = false;

    BasicRanges() = default;
    static Ranges& GetRanges() noexcept;
    static void BuildRanges(Ranges &ranges) noexcept;
    static void SpawnRanges(Ranges &ranges, int, int, int, int) noexcept;
};

class AllCases {
public:
    void Build() noexcept;
    bool IsAvailable() const noexcept;
    const RangesUnion& Fetch() noexcept;
    void BuildParallel(Executor &&executor) noexcept;

    DISALLOW_COPY_AND_ASSIGN(AllCases);
    static AllCases& Instance() noexcept;

private:
    std::mutex building_;
    bool available_ = false;

    AllCases() = default;
    static RangesUnion& GetCases() noexcept;
    static void BuildCases(int head, Ranges &release) noexcept;
};

} // namespace cases
} // namespace klotski
