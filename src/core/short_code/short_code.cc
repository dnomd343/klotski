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
}

} // namespace codec
} // namespace klotski
