#pragma once

#include <vector>
#include "tiny_pool.h"

typedef std::vector<std::pair<uint64_t, uint64_t>> range_split_t;

inline range_split_t range_split(uint64_t start, uint64_t end, uint64_t size) {
    uint64_t rear = end - (end - start) % size; // (rear - start) % size == 0
    range_split_t ranges;
    for (uint64_t i = start; i < rear; i += size) {
        ranges.emplace_back(i, i + size);
    }
    if (rear != end) {
        ranges.emplace_back(rear, end);
    }
    return ranges;
}
