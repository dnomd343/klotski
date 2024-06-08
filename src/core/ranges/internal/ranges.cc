#pragma once

#include "common_code/common_code.h"

using klotski::cases::Ranges;
using klotski::codec::CommonCode;
using klotski::cases::RangesUnion;

inline void Ranges::reverse() {
    for (auto &x : *this) {
        x = range_reverse(x);
    }
}

inline std::vector<CommonCode> RangesUnion::codes() const {
    std::vector<CommonCode> codes;

    codes.reserve(0); // TODO: cal sum

    for (uint64_t head = 0; head < 16; ++head) {

        if (head % 4 == 3) {
            continue;
        }

        for (auto range : (*this)[head]) {
            auto kk = head << 32 | range;
            codes.emplace_back(CommonCode::unsafe_create(kk));

        }

    }

    return codes;
}
