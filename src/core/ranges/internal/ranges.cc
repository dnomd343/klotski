#include <span>

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

KLSK_INLINE CommonCode RangesUnion::operator[](size_type n) const {
    if (n < ranges(0).size()) {
        return CommonCode::unsafe_create(ranges(0)[n]);
    }
    n -= ranges(0).size();

    KLSK_UNROLL(Heads.size() - 2)
    for (const uint64_t head : std::span {Heads.data() + 1, Heads.size() - 2}) {
        if (n < ranges(head).size()) {
            return CommonCode::unsafe_create(head << 32 | ranges(head)[n]);
        }
        n -= ranges(head).size();
    }

    constexpr auto head = static_cast<uint64_t>(0xE);
    return CommonCode::unsafe_create(head << 32 | ranges(0xE)[n]);
}
