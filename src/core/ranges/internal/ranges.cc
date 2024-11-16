#include "ranges/ranges.h"
#include "common_code/common_code.h"

using klotski::cases::Ranges;
using klotski::codec::CommonCode;
using klotski::cases::RangesUnion;

RangesUnion& RangesUnion::operator+=(const RangesUnion &ranges_union) {
    for (const auto head : Heads) {
        ranges(head) += ranges_union.ranges(head);
    }
    return *this;
}

std::vector<CommonCode> RangesUnion::codes() const {
    std::vector<CommonCode> codes;
    codes.reserve(size());
    for (const auto head : Heads) {
        // for (const auto range : (*this)[head]) {
        for (const auto range : ranges(head)) {
            codes.emplace_back(CommonCode::unsafe_create(head << 32 | range));
        }
    }
    // TODO: try using std::views
    return codes;
}

KLSK_INLINE CommonCode RangesUnion::operator[](size_type n) const {
    // uint64_t head = 0;
    // while (n >= ranges(head).size()) {
    //     n -= ranges(head).size();
    //     ++head;
    // }
    // KLSK_ASSUME(head < 16);
    // return CommonCode::unsafe_create(head << 32 | ranges(head)[n]);

    if (n < ranges(0).size()) {
        return CommonCode::unsafe_create(ranges(0)[n]);
    }
    n -= ranges(0).size();

    KLSK_UNROLL(sizeof(Heads) - 2)
    for (const uint64_t head : std::to_array({0x1, 0x2, 0x4, 0x5, 0x6, 0x8, 0x9, 0xA, 0xC, 0xD})) {
        if (n < ranges(head).size()) {
            return CommonCode::unsafe_create(head << 32 | ranges(head)[n]);
        }
        n -= ranges(head).size();
    }

    return CommonCode::unsafe_create((uint64_t)0xE << 32 | ranges(0xE)[n]);
}
