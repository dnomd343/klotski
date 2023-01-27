#include "klotski.h"
#include "short_code.h"

using klotski::RawCode;
using klotski::ShortCode;
using klotski::CommonCode;

bool raw_code_check(uint64_t code) {
    return RawCode::check(code);
}

bool short_code_check(uint32_t code) {
    return ShortCode::check(code);
}

bool common_code_check(uint64_t code) {
    return CommonCode::check(code);
}

void short_code_speed_up() {
    ShortCode::speed_up(ShortCode::NORMAL);
}

void short_code_speed_up_fast() {
    ShortCode::speed_up(ShortCode::FAST);
}
