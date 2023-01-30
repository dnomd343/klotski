#include <cstring>
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

uint32_t raw_code_to_short_code_unsafe(uint64_t raw_code) {
    return RawCode::unsafe_create(raw_code)
        .to_common_code().to_short_code().unwrap();
}

uint64_t short_code_to_raw_code_unsafe(uint32_t short_code) {
    return ShortCode::unsafe_create(short_code)
        .to_common_code().to_raw_code().unwrap();
}

uint64_t raw_code_to_common_code_unsafe(uint64_t raw_code) {
    return RawCode::unsafe_create(raw_code).to_common_code().unwrap();
}

uint64_t common_code_to_raw_code_unsafe(uint64_t common_code) {
    return CommonCode::unsafe_create(common_code).to_raw_code().unwrap();
}

uint64_t short_code_to_common_code_unsafe(uint32_t short_code) {
    return ShortCode::unsafe_create(short_code).to_common_code().unwrap();
}

uint32_t common_code_to_short_code_unsafe(uint64_t common_code) {
    return CommonCode::unsafe_create(common_code).to_short_code().unwrap();
}

////////////////////////////////////////////////////////////////////////////////

const uint32_t SHORT_CODE_STR_SIZE = 6;

bool short_code_to_string(uint32_t short_code, char short_code_str[]) {
    if (!ShortCode::check(short_code)) {
        return false;
    }
    std::string str = ShortCode::unsafe_create(short_code).to_string();
    strcpy(short_code_str, str.c_str());
    return true;
}

bool short_code_from_string(const char short_code_str[], uint32_t *short_code) {
    try {
        *short_code = ShortCode::from_string(short_code_str).unwrap();
    } catch (...) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////

const uint32_t COMMON_CODE_STR_SIZE = 10;

bool common_code_to_string(uint64_t common_code, char common_code_str[]) {
    if (!CommonCode::check(common_code)) {
        return false;
    }
    std::string str = CommonCode::unsafe_create(common_code).to_string(false);
    strcpy(common_code_str, str.c_str());
    return true;
}

bool common_code_to_string_shorten(uint64_t common_code, char common_code_str[]) {
    if (!CommonCode::check(common_code)) {
        return false;
    }
    std::string str = CommonCode::unsafe_create(common_code).to_string(true);
    strcpy(common_code_str, str.c_str());
    return true;
}

bool common_code_from_string(const char common_code_str[], uint64_t *common_code) {
    try {
        *common_code = CommonCode::from_string(common_code_str).unwrap();
    } catch (...) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
