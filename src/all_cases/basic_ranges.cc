#include <queue>
#include <algorithm>
#include "common.h"
#include "basic_ranges.h"

std::mutex BasicRanges::building;
bool BasicRanges::available = false;
std::vector<uint32_t> BasicRanges::data;

inline uint32_t binary_count(uint32_t bin) { // get number of non-zero bits
    bin -= (bin >> 1) & 0x55555555;
    bin = (bin & 0x33333333) + ((bin >> 2) & 0x33333333);
    bin = ((bin >> 4) + bin) & 0x0F0F0F0F;
    bin += bin >> 8;
    bin += bin >> 16;
    return bin & 0b111111;
}

BasicRanges::Status BasicRanges::status() { // get basic ranges status
    if (BasicRanges::available) {
        return AVAILABLE; // basic ranges already built
    }
    if (!BasicRanges::building.try_lock()) { // fail to lock mutex
        return BUILDING; // another thread working
    }
    BasicRanges::building.unlock(); // release mutex
    return NO_INIT;
}

const std::vector<uint32_t>* BasicRanges::fetch() { // get basic ranges content
    if (status() != AVAILABLE) {
        BasicRanges::build(); // basic ranges initialize
    }
    return &BasicRanges::data; // return const ptr
}

void BasicRanges::build() { // ensure that basic ranges available
    if (!BasicRanges::available) {
        if (BasicRanges::building.try_lock()) { // mutex lock success
            build_data(); // start build process
            BasicRanges::available = true; // set available flag
        } else {
            BasicRanges::building.lock(); // blocking waiting
        }
        BasicRanges::building.unlock(); // release mutex
    }
}

#include <iostream>

void BasicRanges::sort_data(std::vector<int> &flags, std::vector<uint32_t> &raw) {
    struct heap_node {
        uint32_t value;
        int index;
        int limit;
    };

    struct compare {
        bool operator() (heap_node n1, heap_node n2) {
            return n1.value > n2.value;
        }
    };
    std::priority_queue<heap_node, std::vector<heap_node>, compare> min_heap;

    for (auto i = 0; i < flags.size() - 1; ++i) {
        min_heap.push({
            .value = raw[flags[i]],
            .index = flags[i],
            .limit = flags[i + 1] - 1,
        });
    }

    while (!min_heap.empty()) {
        auto current = min_heap.top();
        min_heap.pop();
        data.emplace_back(current.value);
        if (current.index != current.limit) {
            min_heap.push({
                .value = raw[current.index + 1],
                .index = current.index + 1,
                .limit = current.limit,
            });
        }
    }
}

void BasicRanges::build_data() { // build basic ranges
    BasicRanges::data.reserve(BASIC_RANGES_SIZE); // memory pre-allocated

    std::vector<uint32_t> raw_data;
    raw_data.reserve(BASIC_RANGES_SIZE);

    std::vector<int> start_points;

    for (int n = 0; n <= 7; ++n) { // number of 1x2 and 2x1 block -> 0 ~ 7
        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) { // number of 2x1 block -> 0 ~ n
            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) { // number of 1x1 block -> 0 ~ (14 - 2n)

                start_points.emplace_back(raw_data.size());

                generate(raw_data, generate_t { // generate target ranges
                    .n1 = 16 - n * 2 - n_1x1, /// space -> 00
                    .n2 = n - n_2x1, /// 1x2 -> 01
                    .n3 = n_2x1, /// 2x1 -> 10
                    .n4 = n_1x1, /// 1x1 -> 11
                });
            }
        }
    }

    start_points.emplace_back(raw_data.size());

//    std::sort(BasicRanges::data.begin(), BasicRanges::data.end()); // sort basic ranges

    BasicRanges::sort_data(start_points, raw_data);

    std::cout << "size: " << BasicRanges::data.size() << std::endl;

//    std::sort(raw_data.begin(), raw_data.end());
//    std::cout << raw_data.size() << std::endl;

//    for (auto &range : BasicRanges::data) {
//        range = Common::range_reverse(range); // basic ranges reverse
//    }
}

void BasicRanges::generate(std::vector<uint32_t> &release, generate_t info) { // generate specific basic ranges
    constexpr uint32_t MASK_01 = (uint32_t)0b01 << 30;
    constexpr uint32_t MASK_10 = (uint32_t)0b10 << 30;
    constexpr uint32_t MASK_11 = (uint32_t)0b11 << 30;

    /// n4       n3       n2       n1
    /// 00000000 00000000 00000000 00000000 (32-bits)
    struct build_t {
        uint32_t nx;
        uint32_t prefix;
        int offset;
    };

    std::queue<build_t> cache;
    cache.emplace(build_t {
        .nx = static_cast<uint32_t>(info.n1 | info.n2 << 8 | info.n3 << 16 | info.n4 << 24),
        .prefix = 0x00000000,
        .offset = 0,
    });

    while (!cache.empty()) { // queue without elements
        auto current = cache.front();
        if (!current.nx) { // both n1, n2, n3, n4 -> 0
            release.emplace_back(current.prefix); // release prefix as basic range
            cache.pop();
            continue; // skip search
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
        cache.pop(); // remove searched case
    }
}
