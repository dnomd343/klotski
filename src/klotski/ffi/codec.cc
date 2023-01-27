#include "klotski.h"
#include "all_cases.h"
#include "short_code.h"

using klotski::RawCode;
using klotski::ShortCode;
using klotski::CommonCode;

using klotski::AllCases;
using klotski::BasicRanges;

////////////////////////////////////////////////////////////////////////////////

void short_code_enable() {
    ShortCode::speed_up(ShortCode::NORMAL);
}

void short_code_enable_fast() {
    ShortCode::speed_up(ShortCode::FAST);
}

bool is_short_code_available() {
    return BasicRanges::status() == BasicRanges::AVAILABLE;
}

bool is_short_code_available_fast() {
    return AllCases::status() == AllCases::AVAILABLE;
}

////////////////////////////////////////////////////////////////////////////////

bool raw_code_check(uint64_t raw_code) {
    return RawCode::check(raw_code);
}

bool short_code_check(uint32_t short_code) {
    return ShortCode::check(short_code);
}

bool common_code_check(uint64_t common_code) {
    return CommonCode::check(common_code);
}

////////////////////////////////////////////////////////////////////////////////

bool raw_code_to_short_code(uint64_t raw_code, uint32_t *short_code) {
    if (!RawCode::check(raw_code)) {
        return false;
    }
    *short_code = ShortCode::from_common_code(
        CommonCode::from_raw_code(RawCode::unsafe_create(raw_code))
    ).unwrap();
    return true;
}

bool short_code_to_raw_code(uint32_t short_code, uint64_t *raw_code) {
    if (!ShortCode::check(short_code)) {
        return false;
    }
    *raw_code = RawCode::from_common_code(
        CommonCode::from_short_code(ShortCode::unsafe_create(short_code))
    ).unwrap();
    return true;
}

bool raw_code_to_common_code(uint64_t raw_code, uint64_t *common_code) {
    if (!RawCode::check(raw_code)) {
        return false;
    }
    *common_code = CommonCode::from_raw_code(
        RawCode::unsafe_create(raw_code)
    ).unwrap();
    return true;
}

bool common_code_to_raw_code(uint64_t common_code, uint64_t *raw_code) {
    if (!CommonCode::check(common_code)) {
        return false;
    }
    *raw_code = RawCode::from_common_code(
        CommonCode::unsafe_create(common_code)
    ).unwrap();
    return true;
}

bool short_code_to_common_code(uint32_t short_code, uint64_t *common_code) {
    if (!ShortCode::check(short_code)) {
        return false;
    }
    *common_code = CommonCode::from_short_code(
        ShortCode::unsafe_create(short_code)
    ).unwrap();
    return true;
}

bool common_code_to_short_code(uint64_t common_code, uint32_t *short_code) {
    if (!CommonCode::check(common_code)) {
        return false;
    }
    *short_code = ShortCode::from_common_code(
        CommonCode::unsafe_create(common_code)
    ).unwrap();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
