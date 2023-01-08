#include "short_code.h"

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

#include <iostream>

std::string ShortCode::to_string() const {

    // this->code ==> std::string

    std::cout << "short code: " << code << std::endl;

    return "";
}
