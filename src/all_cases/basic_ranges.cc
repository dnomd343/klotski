#include <algorithm>
#include "common.h"
#include "basic_ranges.h"

std::mutex BasicRanges::basic_ranges_building;
std::vector<uint32_t> BasicRanges::basic_ranges;
bool BasicRanges::basic_ranges_available = false;

inline uint32_t binary_count(uint32_t bin) { // get number of non-zero bits
    bin -= (bin >> 1) & 0x55555555;
    bin = (bin & 0x33333333) + ((bin >> 2) & 0x33333333);
    bin = ((bin >> 4) + bin) & 0x0F0F0F0F;
    bin += bin >> 8;
    bin += bin >> 16;
    return bin & 0b111111;
}

BasicRanges::Status BasicRanges::basic_ranges_status() { // get basic ranges status
    if (basic_ranges_available) {
        return AVAILABLE; // basic ranges already built
    }
    if (!basic_ranges_building.try_lock()) { // fail to lock mutex -> another thread working
        return BUILDING;
    }
    basic_ranges_building.unlock(); // release mutex
    return NO_INIT;
}

const std::vector<uint32_t>* BasicRanges::get_basic_ranges() { // get const ptr of basic ranges
    if (basic_ranges.empty()) {
        build_basic_ranges(); // basic ranges initialize
    }
    return &basic_ranges; // return ptr
}

void BasicRanges::build_basic_ranges() { // build basic ranges
    if (basic_ranges_available) {
        return; // basic ranges already built
    }
    if (basic_ranges_building.try_lock()) { // lock success -> start building
        /// 0x0 -> 00 | 1x2 -> 01 | 2x1 -> 10 | 1x1 -> 11
        for (int n = 0; n <= 7; ++n) { // number of 1x2 and 2x1 block -> 0 ~ 7
            for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) { // number of 2x1 block -> 0 ~ n
                for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) { // number of 1x1 block -> 0 ~ (14 - 2n)
                    int n_1x2 = n - n_2x1;
                    int n_space = 16 - n * 2 - n_1x1;
                    generate_ranges(n_space, n_1x2, n_2x1, n_1x1); // generate target ranges
                }
            }
        }
        std::sort(basic_ranges.begin(), basic_ranges.end()); // sort basic ranges
        for (uint32_t &range : basic_ranges) {
            range = Common::range_reverse(range); // basic ranges reverse
        }
        basic_ranges_available = true; // set available flag
    } else { // another thread building
        basic_ranges_building.lock(); // blocking waiting
    }
    basic_ranges_building.unlock();
}

void BasicRanges::generate_ranges(int n1, int n2, int n3, int n4) { // generate specific basic ranges
    int len, limit;
    constexpr uint32_t MASK_01 = 0b01 << 30;
    constexpr uint32_t MASK_10 = 0b10 << 30;
    constexpr uint32_t MASK_11 = 0b11 << 30;
    std::vector<uint32_t> cache_1, cache_2;

    len = n1 + n2;
    limit = 0b1 << len;
    for (uint32_t bin = 0; bin < limit; ++bin) {
        if (binary_count(bin) != n2) { // skip binary without `n2` non-zero bits
            continue;
        }
        uint32_t range = 0;
        for (int i = 0; i < len; ++i) { // generate range base on binary value
            range >>= 2;
            if ((bin >> i) & 0b1) { // non-zero bit
                range |= MASK_01;
            }
        }
        cache_1.emplace_back(range); // insert into first layer
    }

    len += n3;
    limit <<= n3;
    for (uint32_t bin = 0; bin < limit; ++bin) {
        if (binary_count(bin) != n3) { // skip binary without `n3` non-zero bits
            continue;
        }
        for (uint32_t base : cache_1) { // traverse first layer
            uint32_t range = 0;
            for (int i = 0; i < len; ++i) { // generate range base on binary value
                if ((bin >> i) & 0b1) { // non-zero bit
                    (range >>= 2) |= MASK_10;
                    continue;
                }
                (range >>= 2) |= base & MASK_11;
                base <<= 2;
            }
            cache_2.emplace_back(range); // insert into second layer
        }
    }

    len += n4;
    limit <<= n4;
    for (uint32_t bin = 0; bin < limit; ++bin) {
        if (binary_count(bin) != n4) { // skip binary without `n4` non-zero bits
            continue;
        }
        for (uint32_t base : cache_2) { // traverse second layer
            uint32_t range = 0;
            for (int i = 0; i < len; ++i) { // generate range base on binary value
                if ((bin >> i) & 0b1) { // non-zero bit
                    (range >>= 2) |= MASK_11;
                    continue;
                }
                (range >>= 2) |= base & MASK_11;
                base <<= 2;
            }
            basic_ranges.emplace_back(range); // insert into release
        }
    }
}
