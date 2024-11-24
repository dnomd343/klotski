#include "ranges/ranges.h"
#include "common_code/common_code.h"

using klotski::cases::Ranges;
using klotski::codec::CommonCode;
using klotski::cases::RangesUnion;

std::vector<CommonCode> RangesUnion::codes() const {
    std::vector<CommonCode> codes;
    codes.reserve(size());
    for (const auto head : Heads) {
        for (const auto range : ranges(head)) {
            codes.emplace_back(CommonCode::unsafe_create(head << 32 | range));
        }
    }
    // TODO: try using std::views
    return codes;
}

void Ranges::derive(const int head, Ranges &output) const {
    const uint32_t max_val = range_reverse(this->back());
    for (uint32_t index = 0; index < size(); ++index) {
        if (const auto offset = check(head, (*this)[index])) { // invalid case
            ///         !! <- broken
            /// ( xx xx xx ) xx xx xx ... [reversed range]
            ///         +1   00 00 00 ...     (delta)
            const uint32_t delta = 1U << (32 - offset * 2); // distance to next possible range
            const auto min_next = delta + (range_reverse((*this)[index]) & ~(delta - 1));
            if (min_next > max_val) {
                break; // index has overflowed
            }
            while (range_reverse((*this)[++index]) < min_next) {} // located next range
            --index;
            continue;
        }
        output.emplace_back(range_reverse((*this)[index])); // release valid case
    }
}
