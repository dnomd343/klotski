#include <queue>
#include <algorithm>
#include "common.h"
#include "basic_ranges.h"

namespace klotski {

using Common::range_reverse;

/// static variable initialize
std::mutex BasicRanges::building_;
bool BasicRanges::available_ = false;
BasicRanges::basic_ranges_t BasicRanges::data_;

const BasicRanges::basic_ranges_t& BasicRanges::fetch() {
    if (status() != AVAILABLE) {
        build();
    }
    return data_; // return const reference
}

BasicRanges::Status BasicRanges::status() noexcept {
    if (available_) {
        return AVAILABLE; // data already built
    }
    if (!building_.try_lock()) { // fail to lock mutex
        return BUILDING; // another thread working
    }
    building_.unlock(); // release mutex
    return NOT_INIT;
}

void BasicRanges::build() { // ensure that data is available
    if (!available_) {
        if (building_.try_lock()) { // mutex lock success
            build_data(); // start build process
            available_ = true;
        } else {
            building_.lock(); // blocking waiting
        }
        building_.unlock(); // release mutex
    }
}

/// Search and sort all possible basic-ranges combinations.
void BasicRanges::build_data() {
    data_.reserve(BASIC_RANGES_SIZE); // memory pre-allocated

    /// This will create 204 different combinations.
    for (int n = 0; n <= 7; ++n) // number of 1x2 and 2x1 block -> 0 ~ 7
        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) // number of 2x1 block -> 0 ~ n
            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) // number of 1x1 block -> 0 ~ (14 - 2n)
                generate(data_, generate_t { // generate target ranges
                    .n1 = 16 - n * 2 - n_1x1, /// space -> 00
                    .n2 = n - n_2x1, /// 1x2 -> 01
                    .n3 = n_2x1, /// 2x1 -> 10
                    .n4 = n_1x1, /// 1x1 -> 11
                });

    /// NOTE: multiple sets of ordered data -> use merge sort instead of quick sort
    std::stable_sort(data_.begin(), data_.end()); // sort basic ranges
    for (auto &range : data_) {
        range = range_reverse(range); // basic ranges reversed
    }
}

/// Generate basic-ranges of specific type.
void BasicRanges::generate(basic_ranges_t &release, generate_t info) {
    constexpr auto MASK_01 = (uint32_t)0b01 << 30;
    constexpr auto MASK_10 = (uint32_t)0b10 << 30;
    constexpr auto MASK_11 = (uint32_t)0b11 << 30;

    /// nx:  n4       n3       n2       n1
    ///      00000000 00000000 00000000 00000000 (32-bit)
    struct build_t {
        uint32_t nx; // (n4, n3, n2, n1)
        uint32_t prefix;
        int offset; // prefix length
    };

    std::queue<build_t> cache;
    cache.emplace(build_t { // setup info
        .nx = static_cast<uint32_t>(info.n1 | info.n2 << 8 | info.n3 << 16 | info.n4 << 24),
        .prefix = 0x00000000,
        .offset = 0,
    });

    while (!cache.empty()) { // queue without element -> work complete
        auto current = cache.front();
        if (!current.nx) { // both n1, n2, n3, n4 -> 0
            release.emplace_back(current.prefix); // release prefix as basic range
            cache.pop();
            continue; // skip current search
        }
        if (current.nx & 0xFF) { // n1 -> `00`
            cache.emplace(build_t {
                .nx = current.nx - 0x01, // --n1
                .prefix = current.prefix,
                .offset = current.offset + 2,
            });
        }
        if (current.nx & 0xFF00) { // n2 -> `01`
            cache.emplace(build_t {
                .nx = current.nx - 0x0100, // --n2
                .prefix = current.prefix | (MASK_01 >> current.offset),
                .offset = current.offset + 2,
            });
        }
        if (current.nx & 0xFF0000) { // n3 -> `10`
            cache.emplace(build_t {
                .nx = current.nx - 0x010000, // --n3
                .prefix = current.prefix | (MASK_10 >> current.offset),
                .offset = current.offset + 2,
            });
        }
        if (current.nx & 0xFF000000) { // n4 -> `11`
            cache.emplace(build_t {
                .nx = current.nx - 0x01000000, // --n4
                .prefix = current.prefix | (MASK_11 >> current.offset),
                .offset = current.offset + 2,
            });
        }
        cache.pop(); // remove handled case
    }
}

} // namespace klotski
