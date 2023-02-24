#include "raw_code.h"

using klotski::RawCode;

/// Mirror convert functions
RawCode RawCode::to_vertical_mirror() const {
    return RawCode::unsafe_create(vertical_mirror(code));
}

RawCode RawCode::to_horizontal_mirror() const {
    return RawCode::unsafe_create(horizontal_mirror(code));
}

/// Mirror check functions
bool RawCode::is_vertical_mirror() const {

    // TODO: vertical mirror check

    return false;
}

bool RawCode::is_horizontal_mirror() const {

    // TODO: horizontal mirror check

    return false;
}

bool RawCode::is_vertical_mirror(RawCode &&raw_code) const {

    // TODO: vertical mirror check

    return false;
}

bool RawCode::is_vertical_mirror(const RawCode &raw_code) const {

    // TODO: vertical mirror check

    return false;
}

bool RawCode::is_horizontal_mirror(RawCode &&raw_code) const {

    // TODO: horizontal mirror check

    return false;
}

bool RawCode::is_horizontal_mirror(const RawCode &raw_code) const {

    // TODO: horizontal mirror check

    return false;
}

/// Basic mirror convert
uint64_t RawCode::vertical_mirror(uint64_t raw_code) {

    // TODO: vertical mirror convert

    return 0;
}

uint64_t RawCode::horizontal_mirror(uint64_t raw_code) {

    // TODO: horizontal mirror convert

    return 0;
}
