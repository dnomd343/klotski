#include "ranges/ranges.h"
#include "common_code/common_code.h"

using klotski::cases::Ranges;
using klotski::codec::CommonCode;
using klotski::cases::RangesUnion;

static constexpr auto heads = std::to_array<uint64_t>({
    0x0, 0x1, 0x2, 0x4, 0x5, 0x6,
    0x8, 0x9, 0xA, 0xC, 0xD, 0xE,
});

void Ranges::reverse() {
    for (auto &x : *this) {
        x = range_reverse(x);
    }
}

Ranges& Ranges::operator+=(const Ranges &ranges) {
    this->insert(this->end(), ranges.begin(), ranges.end());
    return *this;
}

RangesUnion& RangesUnion::operator+=(const RangesUnion &ranges_union) {
    for (const auto head : heads) {
        (*this)[head] += ranges_union[head];
    }
    return *this;
}

std::vector<CommonCode> RangesUnion::codes() const {
    size_type size = 0;
    for (const auto head : heads) {
        size += (*this)[head].size();
    }

    std::vector<CommonCode> codes;
    codes.reserve(size);
    for (const auto head : heads) {
        for (const auto range : (*this)[head]) {
            codes.emplace_back(CommonCode::unsafe_create(head << 32 | range));
        }
    }
    return codes;
}
