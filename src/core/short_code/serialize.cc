#include "short_code.h"
#include "serialize_chars.h"

namespace klotski {
namespace codec {

/// --------------------------- ShortCode to String ---------------------------

std::string ShortCode::to_string() const noexcept { // encode as 5-bits string
    return string_encode(code_);
}

/// ----------------------------- Basic Functions -----------------------------

std::string ShortCode::string_encode(uint32_t short_code) noexcept { // encode as 5-bits string
    char result[6]; // short code length 5
    result[5] = '\0'; // string ending flag
    for (int n = 0; n < 5; ++n) {
        result[4 - n] = SHORT_CODE_TABLE[short_code & 0b11111]; // aka _ % 32
        short_code >>= 5; // aka _ / 32
    }
    return result;
}

std::optional<uint32_t> ShortCode::string_decode(const std::string &short_code) noexcept { // 5-bits string decode
    if (short_code.length() != 5) { // check string length
        return std::nullopt;
    }
    uint64_t result = 0;
    for (auto bit : short_code) {
        result <<= 5; // aka _ * 32
        if (bit >= 'a' && bit <= 'z') {
            bit -= 32; // convert to uppercase
        }
        if (bit < '1' || bit > 'Z') { // invalid characters
            return std::nullopt;
        }
        result += (bit = SHORT_CODE_TABLE_REV[bit - 49]); // table convert
        if (bit == -1) { // invalid character
            return std::nullopt;
        }
    }
    if (!ShortCode::check(result)) { // check converted short code
        return std::nullopt;
    }
    return result; // apply convert result
}

} // namespace codec
} // namespace klotski
