#include "ranges/ranges.h"
#include "common_code/common_code.h"

using klotski::cases::Ranges;
using klotski::codec::CommonCode;
using klotski::cases::RangesUnion;

RangesUnion& RangesUnion::operator+=(const RangesUnion &ranges_union) {
    for (const auto head : Heads) {
        // (*this)[head] += ranges_union[head];
        std::array<Ranges, 16>::operator[](head) += ranges_union.std::array<Ranges, 16>::operator[](head);
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
    uint64_t head = 0;
    while (n >= ranges(head).size()) {
        n -= ranges(head).size();
        ++head;
    }
    KLSK_ASSUME(head < 16);
    return CommonCode::unsafe_create(head << 32 | ranges(head)[n]);

    // size_t head = 0;
    // if (n < ranges(head).size()) {
    //     return CommonCode::unsafe_create(head << 32 | ranges(head)[n]); // 0
    // }
    //
    // n -= ranges(head).size();
    // ++head;
    // if (n < ranges(head).size()) {
    //     return ranges(head)[n]; // 1
    // }
    //
    // n -= ranges(head).size();
    // ++head;
    // if (n < ranges(head).size()) {
    //     return ranges(head)[n]; // 2
    // }
    //
    // n -= ranges(head).size();
    // ++head;
    // if (n < ranges(head).size()) {
    //     return ranges(head)[n]; // 3
    // }
    //
    // n -= ranges(head).size();
    // ++head;
    // if (n < ranges(head).size()) {
    //     return ranges(head)[n]; // 4
    // }
    //
    // n -= ranges(head).size();
    // ++head;
    // if (n < ranges(head).size()) {
    //     return ranges(head)[n]; // 5
    // }
    //
    // n -= ranges(head).size();
    // ++head;
    // if (n < ranges(head).size()) {
    //     return ranges(head)[n]; // 6
    // }
    //
    // n -= ranges(head).size();
    // ++head;
    // if (n < ranges(head).size()) {
    //     return ranges(head)[n]; // 7
    // }
    //
    // n -= ranges(head).size();
    // ++head;
    // if (n < ranges(head).size()) {
    //     return ranges(head)[n]; // 8
    // }
    //
    // n -= ranges(head).size();
    // ++head;
    // if (n < ranges(head).size()) {
    //     return ranges(head)[n]; // 9
    // }
    //
    // n -= ranges(head).size();
    // ++head;
    // if (n < ranges(head).size()) {
    //     return ranges(head)[n]; // 10
    // }
    //
    // n -= ranges(head).size();
    // ++head;
    // if (n < ranges(head).size()) {
    //     return ranges(head)[n]; // 11
    // }
    //
    // n -= ranges(head).size();
    // ++head;
    // if (n < ranges(head).size()) {
    //     return ranges(head)[n]; // 12
    // }
    //
    // n -= ranges(head).size();
    // ++head;
    // if (n < ranges(head).size()) {
    //     return ranges(head)[n]; // 13
    // }
    //
    // n -= ranges(head).size();
    // ++head;
    // if (n < ranges(head).size()) {
    //     return ranges(head)[n]; // 14
    // }
    //
    // n -= ranges(head).size();
    // ++head;
    // if (n < ranges(head).size()) {
    //     return ranges(head)[n]; // 15
    // }

}
