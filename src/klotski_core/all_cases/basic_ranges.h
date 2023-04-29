#pragma once

/// Based on the requirements of valid klotski, the `2x2` block that must exist
/// and only one, witch will occupy 4 empty slots, and the remaining 16 slots
/// will be allocated to space, `1x2`, `2x1` and `1x1`. Then, according to the
/// rules of CommonCode, they are coded as `00` `01` `10` `11` respectively, and
/// the remaining positions are filled with `0` and stored as 32-bit variables.

/// As we all know, a space or `1x1` block will occupy 1 slot, `1x2` or `2x1`
/// block will occupy 2 slots, and together they fill 16 positions, so all
/// possible combinations can be calculated, this number is 204. Each combination
/// can produce different permutations. After verification, there are a total of
/// 7311921 possible permutations. The goal of BasicRanges is to find these
/// permutations, sort them and store them in a `uint32_t` array.

/// In terms of algorithms, there are two options: the first is to generate
/// out-of-order data and then quickly arrange them; the second is to generate
/// ordered data for 204 combinations, and then merge and sort them. After testing,
/// the former is faster in generation (consuming T time), but it will consume
/// more time in sorting (about 7T), and the latter will cost about 2T in
/// generation due to the loss of the tree structure queue. But it can save more
/// time in sorting, which is about 2T, so the second solution will get the result
/// faster.

/// Finally, due to the performance considerations of AllCases, the resulting data
/// will be flipped every two bits, which will not consume too much time (less than
/// 10% of T), but can almost double the speed of the subsequent `check_range`.

#include <mutex>
#include <vector>
#include <cstdint>

namespace klotski {

/// basic ranges count
const uint32_t BASIC_RANGES_SIZE = 7311921;

class BasicRanges {
public:
    /// Three basic states, one-way transition.
    /// {NOT_INIT} -> {BUILDING} -> {AVAILABLE}
    enum Status {
        NOT_INIT,
        BUILDING,
        AVAILABLE,
    };
    typedef std::vector<uint32_t> basic_ranges_t;

    /// Trigger the build process, from `NOT_INIT` to `BUILDING`.
    static void build();

    /// Get current status of BasicRanges.
    static Status status() noexcept;

    /// Blocking access to constructed data.
    static const basic_ranges_t& fetch();

private:
    static bool available_;
    static std::mutex building_;
    static basic_ranges_t data_;

    static void build_data();

public:
    /// The number of types of blocks.
    struct generate_t {
        int n1; // number of `00` -> space
        int n2; // number of `01` -> 1x2 block
        int n3; // number of `10` -> 2x1 block
        int n4; // number of `11` -> 1x1 block
    };

    /// Generate all basic-ranges of the specified type.
    static void generate(basic_ranges_t &release, generate_t info);
};

} // namespace klotski
