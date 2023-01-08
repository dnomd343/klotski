#include <stdexcept>
#include "short_code.h"
#include "short_code_chars.h"

uint32_t ShortCode::unwrap() const {
    return code; // get raw uint32_t code
}

bool ShortCode::check(uint32_t short_code) {
    return short_code < ShortCode::SHORT_CODE_LIMIT; // 0 ~ (SHORT_CODE_LIMIT - 1)
}

ShortCode::ShortCode(uint32_t short_code) {
    if (!ShortCode::check(short_code)) { // check input short code
        throw std::invalid_argument("invalid short code");
    }
    code = short_code;
}

ShortCode::ShortCode(const std::string &short_code_str) { // 5-bits string decode
    if (short_code_str.length() != 5) { // check string length
        throw std::invalid_argument("short code format error");
    }

    uint64_t short_code = 0;
    for (auto bit : short_code_str) {
        short_code *= 32;
        if (bit >= 'a' && bit <= 'z') {
            bit -= 32; // convert to uppercase
        }
        if (bit >= '1' && bit <= 'Z') {
            short_code += (bit = SHORT_CODE_TABLE_REV[bit - 49]); // table convert
            if (bit != -1) {
                continue; // pass check
            }
        }
        throw std::invalid_argument("short code format error"); // unknown characters
    }

    if (!ShortCode::check(short_code)) { // check converted short code
        throw std::invalid_argument("invalid short code");
    }
    code = short_code;
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
