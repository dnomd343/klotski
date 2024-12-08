#pragma once

#include <algorithm>

namespace klotski::codec {

KLSK_INLINE_H uint32_t ShortCode::fast_encode(uint64_t common_code) {
    auto head = common_code >> 32;
    const auto &ranges = (*cases_).ranges(head); // match available ranges
    // TODO: try to narrow the scope by prefix
    auto target = std::lower_bound(ranges.begin(), ranges.end(), (uint32_t)common_code);
    return cases::ALL_CASES_OFFSET[head] + (target - ranges.begin());
}

KLSK_INLINE_H uint64_t ShortCode::fast_decode(uint32_t short_code) {
    auto offset = std::upper_bound(cases::ALL_CASES_OFFSET.begin(), cases::ALL_CASES_OFFSET.end(), short_code) - 1;
    uint64_t head = offset - cases::ALL_CASES_OFFSET.begin();
    // return (head << 32) | AllCases::instance().fetch()[head][short_code - *offset];
    return (head << 32) | (*cases_).ranges(head)[short_code - *offset];
}

} // namespace klotski::codec
