#include <string>
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
static std::string normal_encode(uint64_t common_code) {
    char code_str[9];
    for (int i = 0; i < 9; ++i) {
        code_str[8 - i] = to_hex_char(common_code & 0b1111);
        common_code >>= 4;
    }
    return std::string{code_str, code_str + 9};
}

/// Serialize CommonCode into a variable-length string, removing the trailing zero.
static std::string shorten_encode(uint64_t common_code) {
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

/// Serialize CommonCode to string by shorten option.
std::string CommonCode::string_encode(uint64_t common_code, bool shorten) noexcept {
    if (shorten) {
        if (common_code == 0) {
            return "0"; // special case
        }
        return shorten_encode(common_code);
    }
    return normal_encode(common_code);
}

/// Deserialize CommonCode from string and return std::nullopt on error.
std::optional<uint64_t> CommonCode::string_decode(const std::string &common_code) noexcept {
//uint64_t CommonCode::string_decode(const std::string &common_code) noexcept {
    if (common_code.length() > 9 || common_code.empty()) {
        return std::nullopt; // invalid string length
//        return 0;
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

std::string CommonCode::to_string(bool shorten) const noexcept {
    return string_encode(code_, shorten);
}

} // namespace codec
} // namespace klotski
