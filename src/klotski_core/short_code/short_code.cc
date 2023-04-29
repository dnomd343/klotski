#include "all_cases.h"
#include "short_code.h"

namespace klotski {

bool ShortCode::fast_mode_available_ = false;
bool ShortCode::normal_mode_available_ = false;

ShortCode::ShortCode(uint32_t short_code) {
    if (!ShortCode::check(short_code)) { // check input short code
        throw klotski::ShortCodeExp("short code invalid");
    }
    code_ = short_code;
}

std::ostream& operator<<(std::ostream &out, const ShortCode &self) {
    out << self.to_string() << "(" << self.code_ << ")"; // short code info
    return out;
}

bool ShortCode::check(uint32_t short_code) noexcept {
    return short_code < klotski::SHORT_CODE_LIMIT; // 0 ~ (SHORT_CODE_LIMIT - 1)
}

ShortCode::Mode ShortCode::mode() { // ensure speed up enabled and return current mode
    if (fast_mode_available_) {
        return ShortCode::FAST; // fast mode already enabled
    }
    if (normal_mode_available_) {
        return ShortCode::NORMAL; // normal mode already enabled
    }
    speed_up(ShortCode::Mode::NORMAL); // uninitialized -> enable normal mode
    return ShortCode::Mode::NORMAL;
}

void ShortCode::speed_up(ShortCode::Mode mode) {
    if (fast_mode_available_) {
        return; // fast mode already available
    }
    if (mode == ShortCode::FAST) { // build fast mode data
        AllCases::build(); // blocking function
        fast_mode_available_ = true;
    } else if (!normal_mode_available_) { // build normal mode data
        BasicRanges::build(); // blocking function
        normal_mode_available_ = true;
    }
}

} // namespace klotski
