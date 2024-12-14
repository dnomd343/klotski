#pragma once

namespace klotski::codec {

/// Convert a single hexadecimal digit to a character.
static char to_hex_char(const uint64_t hex_bit) {
    KLSK_ASSUME(hex_bit < 0x10);
    if (hex_bit < 0xA) {
        return static_cast<char>(hex_bit + '0');
    }
    return static_cast<char>(hex_bit + 'A' - 10);
}

constexpr auto CONVERT_TABLE = std::to_array<char>({
    /// -1 * 48
    -1, -1, -1, -1, -1, -1,       // [0, 5]
    -1, -1, -1, -1, -1, -1, -1,   // [6, 12]
    -1, -1, -1, -1, -1, -1, -1,   // [13, 19]
    -1, -1, -1, -1, -1, -1, -1,   // [20, 26]
    -1, -1, -1, -1, -1, -1, -1,   // [27, 33]
    -1, -1, -1, -1, -1, -1, -1,   // [34, 40]
    -1, -1, -1, -1, -1, -1, -1,   // [41, 47]

    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, // [48, 57]

    /// -1 * 7
    -1, -1, -1, -1, -1, -1, -1,   // [58, 64]

    10, 11, 12, 13, 14, 15,       // [65, 70]

    /// -1 * 26
    -1, -1, -1, -1, -1, -1, -1,   // [71, 77]
    -1, -1, -1, -1, -1, -1, -1,   // [78, 84]
    -1, -1, -1, -1, -1, -1, -1,   // [85, 91]
    -1, -1, -1, -1, -1,           // [92, 96]

    10, 11, 12, 13, 14, 15,       // [97, 102]

    /// -1 * 25
    -1, -1, -1, -1, -1, -1, -1,   // [103, 109]
    -1, -1, -1, -1, -1, -1, -1,   // [110, 116]
    -1, -1, -1, -1, -1, -1, -1,   // [117, 123]
    -1, -1, -1, -1,               // [124, 127]

    /// -1 * 128
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
});

inline std::string CommonCode::string_encode(uint64_t common_code) {
    char code_str[9];
    for (int i = 0; i < 9; ++i) {
        code_str[8 - i] = to_hex_char(common_code & 0b1111);
        common_code >>= 4;
    }
    return std::string{code_str, code_str + 9};
}

inline std::string CommonCode::string_encode_shorten(const uint64_t common_code) {
    if (common_code == 0) {
        return "0"; // special case
    }
    auto code = string_encode(common_code);
    code.resize(9 - (std::countr_zero(common_code) >> 2));
    return code;
}

inline std::optional<uint64_t> CommonCode::string_decode(const std::string_view common_code) {
    if (common_code.length() > 9 || common_code.empty()) {
        return std::nullopt; // invalid string length
    }
    uint64_t code = 0;
    for (const uint8_t hex_bit : common_code) {
        const auto val = CONVERT_TABLE[hex_bit]; // table convert
        if (val == -1) {
            return std::nullopt;
        }
        (code <<= 4) |= val;
    }
    return code << (36 - common_code.length() * 4); // low-bits fill with zero
}

} // namespace klotski::codec
