#include "short_code.h"
#include "serialize_chars.h"

using klotski::ShortCode;
using klotski::ShortCodeException;

/// --------------------------- ShortCode to String ---------------------------

std::string ShortCode::to_string() const noexcept { // encode as 5-bits string
    return string_encode(code_);
}

/// --------------------------- String to ShortCode ---------------------------

ShortCode::ShortCode(std::string &&short_code) {
    code_ = string_decode(short_code);
}

ShortCode::ShortCode(const std::string &short_code) {
    code_ = string_decode(short_code);
}

ShortCode ShortCode::from_string(std::string &&short_code) {
    return ShortCode(std::forward<std::string>(short_code));
}

ShortCode ShortCode::from_string(const std::string &short_code) {
    return ShortCode(short_code);
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

uint32_t ShortCode::string_decode(const std::string &short_code) { // 5-bits string decode
    if (short_code.length() != 5) { // check string length
        throw ShortCodeException("short code should length 5");
    }
    uint64_t result = 0;
    for (auto bit : short_code) {
        result <<= 5; // aka _ * 32
        if (bit >= 'a' && bit <= 'z') {
            bit -= 32; // convert to uppercase
        }
        if (bit < '1' || bit > 'Z') { // invalid characters
            throw ShortCodeException("short code with invalid character");
        }
        result += (bit = SHORT_CODE_TABLE_REV[bit - 49]); // table convert
        if (bit == -1) { // invalid character
            throw ShortCodeException("short code with invalid character");
        }
    }
    if (!ShortCode::check(result)) { // check converted short code
        throw ShortCodeException("short code invalid");
    }
    return result; // apply convert result
}
