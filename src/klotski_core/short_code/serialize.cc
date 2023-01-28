#include <stdexcept>
#include "short_code.h"
#include "serialize_chars.h"

using klotski::ShortCode;

ShortCode ShortCode::from_string(const std::string &short_code) {
    return ShortCode(short_code); // convert from string
}

std::string ShortCode::to_string() const { // encode as 5-bits string
    uint32_t short_code = code;
    char result[6]; // short code length 5
    result[5] = '\0'; // string ending flag
    for (int n = 0; n < 5; ++n) {
        result[4 - n] = SHORT_CODE_TABLE[short_code & 0b11111]; // aka _ % 32
        short_code >>= 5; // aka _ / 32
    }
    return result;
}

ShortCode::ShortCode(const std::string &short_code) { // 5-bits string decode
    if (short_code.length() != 5) { // check string length
        throw std::invalid_argument("short code format error");
    }
    uint64_t result = 0;
    for (auto bit : short_code) {
        result <<= 5; // aka _ * 32
        if (bit >= 'a' && bit <= 'z') {
            bit -= 32; // convert to uppercase
        }
        if (bit >= '1' && bit <= 'Z') { // valid characters
            result += (bit = SHORT_CODE_TABLE_REV[bit - 49]); // table convert
            if (bit == -1) {
                throw std::invalid_argument("short code format error"); // unknown characters
            }
        }
    }
    if (!ShortCode::check(result)) { // check converted short code
        throw std::invalid_argument("invalid short code");
    }
    code = result; // apply convert result
}
