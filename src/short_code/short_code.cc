#include "all_cases.h"
#include "short_code.h"

bool ShortCode::fast_mode_available = false;
bool ShortCode::normal_mode_available = false;

uint32_t ShortCode::unwrap() const { // get raw uint32_t code
    return code;
}

ShortCode ShortCode::create(uint32_t short_code) {
    return ShortCode(short_code);
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



void ShortCode::speed_up(ShortCode::Mode mode) {
    if (fast_mode_available) {
        return; // fast mode already available
    }
    if (mode == ShortCode::FAST) { // build fast mode data
//        build_mappings();

        // TODO: confirm AllCases data available
        AllCases::build();

    } else if (mode == ShortCode::NORMAL && !normal_mode_available) { // build normal mode data
        BasicRanges::build(); // blocking function
        normal_mode_available = true;
    }
}

ShortCode::Mode ShortCode::mode() { // ensure speed up enabled and return current mode
    if (fast_mode_available) {
        return ShortCode::FAST; // fast mode already enabled
    }
    if (normal_mode_available) {
        return ShortCode::NORMAL; // normal mode already enabled
    }
    speed_up(ShortCode::Mode::NORMAL); // without initialized -> enter normal mode
    return ShortCode::Mode::NORMAL; // use normal mode
}



