#include "raw_code.h"

using klotski::RawCode;

/// ----------------------------- Mirror Convert ------------------------------

RawCode RawCode::to_vertical_mirror() const {
    return RawCode::unsafe_create(vertical_mirror(code));
}

RawCode RawCode::to_horizontal_mirror() const {
    return RawCode::unsafe_create(horizontal_mirror(code));
}

/// ------------------------------ Mirror Check -------------------------------

bool RawCode::is_vertical_mirror() const {
    return vertical_mirror_check(code);
}

bool RawCode::is_horizontal_mirror() const {
    return horizontal_mirror_check(code);
}

bool RawCode::is_vertical_mirror(RawCode &&raw_code) const {
    return raw_code.unwrap() == vertical_mirror(code);
}

bool RawCode::is_horizontal_mirror(RawCode &&raw_code) const {
    return raw_code.unwrap() == horizontal_mirror(code);
}

bool RawCode::is_vertical_mirror(const RawCode &raw_code) const {
    return raw_code.unwrap() == vertical_mirror(code);
}

bool RawCode::is_horizontal_mirror(const RawCode &raw_code) const {
    return raw_code.unwrap() == horizontal_mirror(code);
}

/// ----------------------------- Basic Functions -----------------------------

uint64_t RawCode::vertical_mirror(uint64_t raw_code) {

    // TODO: vertical mirror convert

    return 0;
}

uint64_t RawCode::horizontal_mirror(uint64_t raw_code) {

    // TODO: horizontal mirror convert

    return 0;
}

bool RawCode::vertical_mirror_check(uint64_t raw_code) {

    // TODO: whether self vertical mirror

    return false;
}

bool RawCode::horizontal_mirror_check(uint64_t raw_code) {

    // TODO: whether self horizontal mirror

    return false;
}
