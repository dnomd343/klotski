#include "common_code.h"

namespace klotski {
namespace codec {

/// Convert a single hexadecimal digit to a character.
inline static char to_hex_char(uint64_t hex_bit) {
    if (hex_bit < 0xA) {
        return char(hex_bit + '0');
    }
    return char(hex_bit + 'A' - 10);
}

/// Serialize CommonCode into a 9-bit length string.
std::string CommonCode::string_encode(uint64_t common_code) noexcept {
    char code_str[9];
    for (int i = 0; i < 9; ++i) {
        code_str[8 - i] = to_hex_char(common_code & 0b1111);
        common_code >>= 4;
    }
    return std::string{code_str, code_str + 9};
}

/// Serialize CommonCode into a variable-length string, removing the trailing zero.
std::string CommonCode::string_encode_shorten(uint64_t common_code) noexcept {
    if (common_code == 0) {
        return "0"; // special case
    }
    int zero_start;
    char code_str[9];
    for (int i = 0; i < 9; ++i) {
        auto hex_bit = common_code >> (32 - i * 4) & 0b1111;
        code_str[i] = to_hex_char(hex_bit);
        if (hex_bit != 0) {
            zero_start = 9;
        } else if (zero_start == 9) {
            zero_start = i;
        }
    }
    return std::string{code_str, code_str + zero_start};
}

/// Deserialize CommonCode from string and return std::nullopt on error.
std::optional<uint64_t> CommonCode::string_decode(const std::string &common_code) noexcept {
    if (common_code.length() > 9 || common_code.empty()) {
        return std::nullopt; // invalid string length
    }
    uint64_t result = 0;
    for (auto hex_bit : common_code) {
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

} // namespace codec
} // namespace klotski
