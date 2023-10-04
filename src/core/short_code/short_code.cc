#include "all_cases.h"
#include "short_code.h"

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

namespace klotski {
namespace codec {

/// Check the validity of the original ShortCode.
bool ShortCode::check(uint32_t short_code) noexcept {
    return short_code < SHORT_CODE_LIMIT; // 0 ~ (SHORT_CODE_LIMIT - 1)
}

void ShortCode::speed_up(bool fast_mode) noexcept {
    if (fast_mode) {
        AllCases::instance().build();
    } else {
        BasicRanges::instance().build();
    }
    // FIXME: setting up `fast_available_`
}

//void ShortCode::speed_up(ShortCode::Mode mode) {
//    if (fast_mode_available_) {
//        return; // fast mode already available
//    }
//    if (mode == ShortCode::FAST) { // build fast mode data
//        AllCases::build(); // blocking function
//        fast_mode_available_ = true;
//    } else if (!normal_mode_available_) { // build normal mode data
//        BasicRanges::build(); // blocking function
//        normal_mode_available_ = true;
//    }
//}

} // namespace codec
} // namespace klotski
