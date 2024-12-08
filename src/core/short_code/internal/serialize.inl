#pragma once

#include <ranges>

#include "serialize_chars.h"

namespace klotski::codec {

KLSK_INLINE_H std::string ShortCode::string_encode(uint32_t short_code) {
    KLSK_ASSUME(short_code < cases::ALL_CASES_NUM_);
    std::array<char, 5> arr {};
    for (auto &c : arr | std::views::reverse) {
        c = SHORT_CODE_TABLE[short_code & 0b11111]; // table convert
        short_code >>= 5;
    }
    return {arr.begin(), arr.end()};
}

KLSK_INLINE_H std::optional<uint32_t> ShortCode::string_decode(const std::string_view short_code) {
    if (short_code.length() != 5) {
        return std::nullopt; // invalid string length
    }
    uint32_t code = 0;
    for (const uint8_t bit : short_code) {
        if (bit > 'z') { // invalid characters
            return std::nullopt;
        }
        if (const auto val = SHORT_CODE_TABLE_REV[bit]; val != -1) {
            (code <<= 5) += val;
            continue;
        }
        return std::nullopt; // invalid character
    }
    if (!check(code)) { // check converted short code
        return std::nullopt;
    }
    return code; // apply convert result
}

} // namespace klotski::codec
