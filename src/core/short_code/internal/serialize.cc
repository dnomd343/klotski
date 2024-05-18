#include "serialize_chars.h"
#include "short_code/short_code.h"

using klotski::codec::ShortCode;

std::string ShortCode::string_encode(uint32_t short_code) {
    char result[5];
    for (int n = 0; n < 5; ++n) {
        result[4 - n] = SHORT_CODE_TABLE[short_code & 0b11111];
        short_code >>= 5;
    }
    return {result, result + 5};
}

std::optional<uint32_t> ShortCode::string_decode(const std::string_view short_code) {
    if (short_code.length() != 5) {
        return std::nullopt; // invalid string length
    }
    uint32_t result = 0;
    for (auto bit : short_code) {
        if (bit < '1' || bit > 'z') { // invalid characters
            return std::nullopt;
        }
        result <<= 5;
        result += (bit = SHORT_CODE_TABLE_REV[bit - 49]); // table convert
        if (bit == -1) { // invalid character
            return std::nullopt;
        }
    }
    if (!check(result)) { // check converted short code
        return std::nullopt;
    }
    return result; // apply convert result
}
