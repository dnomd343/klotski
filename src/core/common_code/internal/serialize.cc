#include "common_code/common_code.h"

namespace klotski::codec {

/// Convert a single hexadecimal digit to a character.
static char to_hex_char(const uint64_t hex_bit) {
    [[assume(hex_bit < 0x10)]];
    if (hex_bit < 0xA) {
        return static_cast<char>(hex_bit + '0');
    }
    return static_cast<char>(hex_bit + 'A' - 10);
}

std::string CommonCode::string_encode(uint64_t common_code) {
    char code_str[9];
    for (int i = 0; i < 9; ++i) {
        code_str[8 - i] = to_hex_char(common_code & 0b1111);
        common_code >>= 4;
    }
    return std::string{code_str, code_str + 9};
}

std::string CommonCode::string_encode_shorten(const uint64_t common_code) {
    if (common_code == 0) {
        return "0"; // special case
    }
    auto code = string_encode(common_code);
    code.resize(9 - (std::countr_zero(common_code) >> 2));
    return code;
}

// TODO: direct table lookup can bring about a 30% improvement,
// TODO:   but it is necessary to confirm the performance of different CPU caches.
std::optional<uint64_t> CommonCode::string_decode(const std::string_view common_code) {
    if (common_code.length() > 9 || common_code.empty()) {
        return std::nullopt; // invalid string length
    }
    uint64_t result = 0;
    for (const auto hex_bit : common_code) {
        if (hex_bit >= '0' && hex_bit <= '9') { // 0 ~ 9
            (result <<= 4) |= (hex_bit - '0');
        } else if (hex_bit >= 'A' && hex_bit <= 'F') { // A ~ F
            (result <<= 4) |= (hex_bit - 'A' + 10);
        } else if (hex_bit >= 'a' && hex_bit <= 'f') { // a ~ f
            (result <<= 4) |= (hex_bit - 'a' + 10);
        } else {
            return std::nullopt; // invalid character
        }
    }
    return result << (36 - common_code.length() * 4); // low-bits fill with zero
}

} // namespace klotski::codec
