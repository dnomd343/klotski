#include "global_utils.h"

range_split_t range_split(uint64_t start, uint64_t end, uint64_t size) {
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

uint32_t thread_num() {

    // TODO: use CPU core number

    return 16;
}
