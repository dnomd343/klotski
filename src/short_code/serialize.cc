#include <stdexcept>
#include "short_code.h"
#include "serialize_chars.h"

ShortCode ShortCode::from_string(const std::string &short_code) {
    return ShortCode(short_code); // convert from string
}

std::string ShortCode::to_string() const { // encode as 5-bits string
    uint32_t short_code = code;
    std::string result(5, '\0'); // short code length 5
    for (int n = 0; n < 5; ++n) {
        uint8_t bit = short_code % 32;
        short_code = (short_code - bit) / 32;
        result[4 - n] = SHORT_CODE_TABLE[bit];
    }
    return result;
}

ShortCode::ShortCode(const std::string &short_code) { // 5-bits string decode
    if (short_code.length() != 5) { // check string length
        throw std::invalid_argument("short code format error");
    }

    uint64_t result = 0;
    for (auto bit : short_code) {
        result *= 32;
        if (bit >= 'a' && bit <= 'z') {
            bit -= 32; // convert to uppercase
        }
        if (bit >= '1' && bit <= 'Z') {
            result += (bit = SHORT_CODE_TABLE_REV[bit - 49]); // table convert
            if (bit != -1) {
                continue; // pass check
            }
        }
        throw std::invalid_argument("short code format error"); // unknown characters
    }

    if (!ShortCode::check(result)) { // check converted short code
        throw std::invalid_argument("invalid short code");
    }
    code = result;
}
