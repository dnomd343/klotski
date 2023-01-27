#include "klotski.h"
#include "short_code.h"

using klotski::ShortCode;

void short_code_speed_up() {
    ShortCode::speed_up(ShortCode::NORMAL);
}

void short_code_speed_up_fast() {
    ShortCode::speed_up(ShortCode::FAST);
}
